# ASM.js implementation of Array-Mapped Tries

The Array-Mapped Trie is one of a number of data structures described by Phil Bagwell in [Fast And Space Efficient Trie Searches][paper]. It performs operations that you'd expect a trie to support, namely insertion, search, and deletion of string keys. It does so in a manner that is very fast and simultanesouly memory-efficient.


This project includes an implementation of Array-Mapped Tries in C++, as well as some minimal glue code to expose an idiomatic JavaScript API to the code as compiled by [emscripten][emscripten].

[paper]: http://infoscience.epfl.ch/record/64394/files/triesearches.pdf
[emscripten]: http://kripken.github.io/emscripten-site/
