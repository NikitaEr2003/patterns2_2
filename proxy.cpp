#include <iostream>
#include <map>
#include <string>

class VeryHeavyDatabase {
 public:
  std::string GetData(const std::string& key) const noexcept {
    return "Very Big Data String: " + key;
  }
};

class CacheProxyDB : VeryHeavyDatabase {
 public:
  explicit CacheProxyDB(VeryHeavyDatabase* real_object)
      : real_db_(real_object) {}
  std::string GetData(const std::string& key) noexcept {
    if (cache_.find(key) == cache_.end()) {
      std::cout << "Get from real object\n";
      cache_[key] = real_db_->GetData(key);
    } else {
      std::cout << "Get from cache\n";
    }
    return cache_.at(key);
  }

 private:
  std::map<std::string, std::string> cache_;
  VeryHeavyDatabase* real_db_;
};

class TestDB : VeryHeavyDatabase {
 public:
  explicit TestDB(VeryHeavyDatabase* real_object) : real_db_(real_object) {}
  std::string GetData(const std::string& key) noexcept { return "test_data\n"; }

 private:
  VeryHeavyDatabase* real_db_;
};

class OneShotDB : VeryHeavyDatabase {
 public:
  size_t m_shots;
  VeryHeavyDatabase* m_real_db;
  std::map<std::string, std::string> m_map;
  explicit OneShotDB(VeryHeavyDatabase* real_object, size_t shots = 1)
      : m_shots(shots), m_real_db(real_object) {}
  std::string GetData(const std::string& key) noexcept {
    if (m_shots > 0) {
      m_shots--;
      return m_map[key];
    }
    return "error";
  }
  void SetKeyAndValue(std::string key, std::string value) {
    m_map[key] = value;
  }
};

int main() {
  auto real_db = VeryHeavyDatabase();
  auto limit_db = OneShotDB(std::addressof(real_db), 3);
  limit_db.SetKeyAndValue("key", "ключ");
  std::cout << limit_db.GetData("key") << std::endl;
  std::cout << limit_db.GetData("key") << std::endl;
  std::cout << limit_db.GetData("key") << std::endl;
  std::cout << limit_db.GetData("key") << std::endl;
  return 0;
}
