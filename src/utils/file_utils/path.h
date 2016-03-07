#pragma once

#include <string>

class Path {
  public:
	static std::string makeAbsolute(const std::string& path);

	Path(const std::string& path);

	std::string getExtension() const;

	Path& makeAbsolute();

	bool exists() const;
	bool isSymbolicLink() const;
	bool isDirectory() const;

	std::string toString() const;
    
    Path& operator+=(const Path& path);
  private:
    std::string path_;
};

Path operator+(const Path& p1, const Path& p2);
