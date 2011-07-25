/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2011 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#ifndef FRENCHLIGHTSTEMMER_H
#define FRENCHLIGHTSTEMMER_H

#include "LuceneContrib.h"
#include "LuceneObject.h"

namespace Lucene
{
    /// This algorithm is updated based on code located at:
    /// http://members.unine.ch/jacques.savoy/clef/
    ///
    /// Full copyright for that code follows:
    ///
    /// Copyright (c) 2005, Jacques Savoy
    /// All rights reserved.
    ///
    /// Redistribution and use in source and binary forms, with or without
    /// modification, are permitted provided that the following conditions are met:
    ///
    /// Redistributions of source code must retain the above copyright notice, this
    /// list of conditions and the following disclaimer. Redistributions in binary
    /// form must reproduce the above copyright notice, this list of conditions and
    /// the following disclaimer in the documentation and/or other materials
    /// provided with the distribution. Neither the name of the author nor the names
    /// of its contributors may be used to endorse or promote products derived from
    /// this software without specific prior written permission.
    ///
    /// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    /// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    /// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    /// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    /// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    /// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    /// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    /// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    /// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    /// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    /// POSSIBILITY OF SUCH DAMAGE.
    ///
    /// Light Stemmer for French.
    ///
    /// This stemmer implements the "UniNE" algorithm in:
    /// Light Stemming Approaches for the French, Portuguese, French and Hungarian
    /// Languages
    ///
    /// Jacques Savoy
    class LPPCONTRIBAPI FrenchLightStemmer : public LuceneObject
    {
    public:
        virtual ~FrenchLightStemmer();

        LUCENE_CLASS(FrenchLightStemmer);

    public:
        int32_t stem(wchar_t* s, int32_t len);

    private:
        int32_t norm(wchar_t* s, int32_t len);
    };
}

#endif
