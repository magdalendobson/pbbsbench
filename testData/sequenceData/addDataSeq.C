// This code is part of the Problem Based Benchmark Suite (PBBS)
// Copyright (c) 2010 Guy Blelloch and the PBBS team
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "sequenceData.h"
#include "common/sequenceIO.h"
#include "common/parse_command_line.h"
#include "parlay/random.h"
using namespace dataGen;
using namespace benchIO;

template <class T2, class T1>
sequence<pair<T1,T2>> addData(T1* A, size_t n, size_t range) {
  parlay::random r(23);
  using par = pair<T1,T2>;
  auto R = parlay::tabulate(n, [&] (size_t i) -> par {
      return std::make_pair(A[i], (T2) (r.ith_rand(i) % range));;
    });
  return R;
}

int main(int argc, char* argv[]) {
  commandLine P(argc,argv, "[-r <range>] [-t {int,double}] <inFile> <outFile>");
  pair<char*,char*> fnames = P.IOFileNames();
  char* ifile = fnames.first;
  char* ofile = fnames.second;
  seqData D = readSequenceFromFile(ifile);
  elementType dt = D.dt;
  size_t n = D.n;
  elementType dataDT = elementTypeFromString(P.getOptionValue("-t","int"));
  if (dataDT == none) dataDT = dt;

  char* rangeString = P.getOptionValue("-r");
  size_t range = n;
  if (rangeString != NULL) range = atoi(rangeString);

  switch(dt) {
  case intType: 
    switch (dataDT) {
    case intType: 
      return writeSequenceToFile(addData<uint>((uint*) D.A, n, range),ofile);
    default:
      cout << "addData: not a valid type" << endl;
      return 1;
    }
  case doubleT: 
    switch (dataDT) {
    case doubleT: 
      return writeSequenceToFile(addData<double>((double*) D.A, n, range),ofile);
    default:
      cout << "addData: not a valid type" << endl;
      return 1;
    }
  case stringT: 
    switch (dataDT) {
    case intType: 
      return writeSequenceToFile(addData<uint>((char**) D.A, n, range),ofile);
    default:
      cout << "addData: not a valid type" << endl;
      return 1;
    }
  default:
    cout << "addData: not a valid type" << endl;
    return 1;
  }
}
