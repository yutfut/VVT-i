// Copyright 2021 nat-s.skv@mail.ru

#include <working_with_filesystem.hpp>

namespace fs = std::filesystem;

namespace fsWorker {

    class FsWorker {
    public:
        FsWorker(const fs::path &path);

        FsWorker(fs::path &&path);

        FsWorker(const FsWorker &sourceFSW);

        FsWorker(FsWorker &&sourceFSW);

        FsWorker &operator=(const FsWorker &) = delete;

        FsWorker &operator=(FsWorker &&) = delete;

        ~FsWorker() = default;

        bool AddUsrFile(const fs::path &filePath);

        bool AddNotAuthUsrFile(const fs::path &filePath);

        std::ifstream GetUsrFile(int userID, const fs::path &filePath);

        std::ifstream GetNotAuthUsrFile(const fs::path &fileName, int uploadDate);

        bool RemoveUsrFile(const fs::path &path);

        bool RemoveNotAuthUsrFile(const fs::path &name);

        bool CreateUsrDir(int userID, const fs::path &dirPath = "");

        bool CreateGroupDir(int groupID, const fs::path &dirPath = "");

        bool RemoveDir(const fs::path &path);

    private:
        static const size_t fileExpirationDate = 10u;
        static const fs::path usersDir;
        static const fs::path groupsDir;
        static const fs::path notAuthDir;
        std::vector<fs::path> notAuthDirs{fileExpirationDate};
    };
}
