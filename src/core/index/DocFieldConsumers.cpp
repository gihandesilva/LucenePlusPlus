/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2011 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "DocFieldConsumers.h"
#include "DocFieldConsumersPerField.h"
#include "DocFieldConsumersPerThread.h"
#include "MiscUtils.h"

namespace Lucene
{
    DocFieldConsumers::DocFieldConsumers(DocFieldConsumerPtr one, DocFieldConsumerPtr two)
    {
        freeCount = 0;
        allocCount = 0;
        docFreeList = Collection<DocFieldConsumersPerDocPtr>::newInstance(1);

        this->one = one;
        this->two = two;
    }

    DocFieldConsumers::~DocFieldConsumers()
    {
    }

    void DocFieldConsumers::setFieldInfos(FieldInfosPtr fieldInfos)
    {
        DocFieldConsumer::setFieldInfos(fieldInfos);
        one->setFieldInfos(fieldInfos);
        two->setFieldInfos(fieldInfos);
    }

    void DocFieldConsumers::flush(MapDocFieldConsumerPerThreadCollectionDocFieldConsumerPerField threadsAndFields, SegmentWriteStatePtr state)
    {
        MapDocFieldConsumerPerThreadCollectionDocFieldConsumerPerField oneThreadsAndFields(MapDocFieldConsumerPerThreadCollectionDocFieldConsumerPerField::newInstance());
        MapDocFieldConsumerPerThreadCollectionDocFieldConsumerPerField twoThreadsAndFields(MapDocFieldConsumerPerThreadCollectionDocFieldConsumerPerField::newInstance());

        for (MapDocFieldConsumerPerThreadCollectionDocFieldConsumerPerField::iterator entry = threadsAndFields.begin(); entry != threadsAndFields.end(); ++entry)
        {
            Collection<DocFieldConsumerPerFieldPtr> oneFields(Collection<DocFieldConsumerPerFieldPtr>::newInstance());
            Collection<DocFieldConsumerPerFieldPtr> twoFields(Collection<DocFieldConsumerPerFieldPtr>::newInstance());

            for (Collection<DocFieldConsumerPerFieldPtr>::iterator perField = entry->second.begin(); perField != entry->second.end(); ++perField)
            {
                oneFields.add(LuceneStaticCast<DocFieldConsumersPerField>(*perField)->one);
                twoFields.add(LuceneStaticCast<DocFieldConsumersPerField>(*perField)->two);
            }

            oneThreadsAndFields.put(LuceneStaticCast<DocFieldConsumersPerThread>(entry->first)->one, oneFields);
            twoThreadsAndFields.put(LuceneStaticCast<DocFieldConsumersPerThread>(entry->first)->two, oneFields);
        }

        one->flush(oneThreadsAndFields, state);
        two->flush(twoThreadsAndFields, state);
    }

    void DocFieldConsumers::closeDocStore(SegmentWriteStatePtr state)
    {
        LuceneException finally;
        try
        {
            one->closeDocStore(state);
        }
        catch (LuceneException& e)
        {
            finally = e;
        }
        try
        {
            two->closeDocStore(state);
        }
        catch (LuceneException& e)
        {
            finally = e;
        }
        finally.throwException();
    }

    bool DocFieldConsumers::freeRAM()
    {
        return (one->freeRAM() || two->freeRAM());
    }

    DocFieldConsumerPerThreadPtr DocFieldConsumers::addThread(DocFieldProcessorPerThreadPtr docFieldProcessorPerThread)
    {
        return newLucene<DocFieldConsumersPerThread>(docFieldProcessorPerThread, LuceneThis(), one->addThread(docFieldProcessorPerThread), two->addThread(docFieldProcessorPerThread));
    }

    DocFieldConsumersPerDocPtr DocFieldConsumers::getPerDoc()
    {
        SyncLock syncLock(this);
        if (freeCount == 0)
        {
            ++allocCount;
            if (allocCount > docFreeList.size())
            {
                // Grow our free list up front to make sure we have enough space to recycle all outstanding
                // PerDoc instances
                BOOST_ASSERT(allocCount == 1 + docFreeList.size());
                docFreeList.resize(MiscUtils::getNextSize(allocCount));
            }
            return newLucene<DocFieldConsumersPerDoc>(LuceneThis());
        }
        else
            return docFreeList[--freeCount];
    }

    void DocFieldConsumers::freePerDoc(DocFieldConsumersPerDocPtr perDoc)
    {
        SyncLock syncLock(this);
        BOOST_ASSERT(freeCount < docFreeList.size());
        docFreeList[freeCount++] = perDoc;
    }

    DocFieldConsumersPerDoc::DocFieldConsumersPerDoc(DocFieldConsumersPtr fieldConsumers)
    {
        this->fieldConsumers = fieldConsumers;
    }

    DocFieldConsumersPerDoc::~DocFieldConsumersPerDoc()
    {
    }

    int64_t DocFieldConsumersPerDoc::sizeInBytes()
    {
        return one->sizeInBytes() + two->sizeInBytes();
    }

    void DocFieldConsumersPerDoc::finish()
    {
        LuceneException finally;
        try
        {
            one->finish();
        }
        catch (LuceneException& e)
        {
            finally = e;
        }
        try
        {
            two->finish();
        }
        catch (LuceneException& e)
        {
            finally = e;
        }
        fieldConsumers->freePerDoc(LuceneThis());
        finally.throwException();
    }

    void DocFieldConsumersPerDoc::abort()
    {
        LuceneException finally;
        try
        {
            one->abort();
        }
        catch (LuceneException& e)
        {
            finally = e;
        }
        try
        {
            two->abort();
        }
        catch (LuceneException& e)
        {
            finally = e;
        }
        fieldConsumers->freePerDoc(LuceneThis());
        finally.throwException();
    }
}