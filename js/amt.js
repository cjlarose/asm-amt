var Module = require('../dist/amt.out.js');

var trie = new Module.Trie();

console.log(trie.contains("hello", 6));
trie.insert("hello", 6);
console.log(trie.contains("hello", 6));

console.log(trie.contains([1,2,3], 7));
trie.insert([1,2,3], 7);
console.log(trie.contains([1,2,3], 7));
