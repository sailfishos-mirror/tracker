Fuzz targets used by [oss-fuzz](https://github.com/google/oss-fuzz/).

## How to add new targets

Add **fuzz_target_name.c** and edit `meson.build` accordingly.

New targets are picked up by oss-fuzz automatically within a day. Targets must not be renamed once added.

Add (optional) **fuzz_target_name.dict** containing keywords and magic bytes.

Add (optional) **fuzz_target_name.corpus** with file names on separate lines. Wildcards `?`, `*` and `**` are supported. Examples below.

```bash
tests/*  # all files in directory tests
tests/** # all files in directory tests and sub-directories
**.rq # all files ending with .rq in the repository
```

Recommended reading: [Fuzz Target](https://llvm.org/docs/LibFuzzer.html#fuzz-target), [Dictionaries](https://llvm.org/docs/LibFuzzer.html#dictionaries), [Corpus](https://llvm.org/docs/LibFuzzer.html#corpus)

## How to reproduce oss-fuzz bugs locally

Build with at least the following flags, choosing a sanitizer as needed. A somewhat recent version of [clang](http://clang.llvm.org/) is recommended.

```bash
$ CC=clang CXX=clang++ meson DIR -Db_sanitize=<address|undefined> -Db_lundef=false
```

Afterwards run the affected target against the provided test case.

```bash
$ DIR/fuzzing/fuzz_target_name FILE
```

#### FAQs

###### What about Memory Sanitizer (MSAN)?

Correct MSAN instrumentation is [difficult to achieve](https://clang.llvm.org/docs/MemorySanitizer.html#handling-external-code) locally, so false positives are very likely to mask the actual bug.

If need be, [you can still reproduce](https://google.github.io/oss-fuzz/advanced-topics/reproducing/#building-using-docker) those bugs with the oss-fuzz provided docker images.

###### There are no file/function names in the stack trace.

`llvm-symbolizer` must be in `PATH`.

###### UndefinedBehavior Sanitizer (UBSAN) doesn't provide a stack trace.

Set environment variable `UBSAN_OPTIONS` to `print_stacktrace=1` prior to running the target.
