var assert = require('chai').assert;
var Module = require('../dist/amt.out.js');

describe('Trie', function() {
  describe('size', function() {
    it('should be zero when initialized', function() {
      var trie = new Module.ArrayMappedTrie();
      assert.equal(trie.size(), 0);
    });
  });
});
