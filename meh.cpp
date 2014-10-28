void CountTrigrams(const std::vector<std::string>& words,
                   map<string, int> * dict,
                   map<int64, int> * result) {
  int64 trigram = 0;
  for (int i = 0; i < words.size(); i++) {
    const& word = words[i];
    int id;
    auto di = dict->find(word);
    if (di == dict->end()) {
      id = dict.size();
      dict[word] = id;
    } else {
      id = di->second;
    }
    trigram = ((trigram << 21) | id) & 0x7fffffffffffffff;
    if (i > 2) {
      auto ti = result->find(trigram);
      if (ti == result->end()) {
        result[trigram] = 1;
      } else {
        ti->second++;
      }
    }
  }
}
