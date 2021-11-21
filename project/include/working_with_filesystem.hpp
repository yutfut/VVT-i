// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_WORKING_WITH_FILESYSTEM_H
#define VVTI_WORKING_WITH_FILESYSTEM_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class IFsSubWorker {
public:
    virtual bool MoveRoot(const fs::path &newRoot, int, std::error_code &ec) noexcept = 0;

    virtual bool MoveFile(const fs::path &filePath, int, std::error_code &ec) const noexcept = 0;

    virtual std::ifstream GetFile(const fs::path &filePath, int) const noexcept = 0;

    virtual bool RemoveFile(const fs::path &path, int, std::error_code &ec) const noexcept = 0;

    virtual bool CreateDir(const fs::path &dirPath, int, std::error_code &ec) const noexcept = 0;

    virtual bool RemoveDir(const fs::path &dirPath, int, std::error_code &ec) const noexcept = 0;
};

class FsSubWorkerNotAuthUsr : public IFsSubWorker {
public:
    explicit FsSubWorkerNotAuthUsr(const fs::path &notAuthDir, size_t fileExpirationDate = fileExpirationDateDefault)
            : notAuthDir(
            notAuthDir), fileExpirationDate(fileExpirationDate), sortDirs{fileExpirationDate} {}

    explicit FsSubWorkerNotAuthUsr(fs::path &&notAuthDir, size_t fileExpirationDate = fileExpirationDateDefault)
            : notAuthDir(
            std::move(notAuthDir)), fileExpirationDate(fileExpirationDate), sortDirs{fileExpirationDate} {}

    FsSubWorkerNotAuthUsr(const FsSubWorkerNotAuthUsr &src) : notAuthDir(src.notAuthDir),
                                                              fileExpirationDate(src.fileExpirationDate),
                                                              sortDirs(src.sortDirs) {}

    FsSubWorkerNotAuthUsr(FsSubWorkerNotAuthUsr &&src) : notAuthDir(std::move(src.notAuthDir)),
                                                         fileExpirationDate(src.fileExpirationDate),
                                                         sortDirs(std::move(src.sortDirs)) {}

    FsSubWorkerNotAuthUsr &operator=(const FsSubWorkerNotAuthUsr &);

    FsSubWorkerNotAuthUsr &operator=(FsSubWorkerNotAuthUsr &&);

    ~FsSubWorkerNotAuthUsr() noexcept = default;

    virtual bool MoveRoot(const fs::path &newNotAuthDir, int, std::error_code &ec) noexcept override;

    bool MoveFile(const fs::path &filePath, int dateAdded, std::error_code &ec) const noexcept override;

    std::ifstream GetFile(const fs::path &filePath, int dateAdded) const noexcept override;

    bool RemoveFile(const fs::path &path, int dateAdded, std::error_code &ec) const noexcept override;

    bool CreateDir(const fs::path &dirPath, int dateAdded, std::error_code &ec) const noexcept override;

    bool RemoveDir(const fs::path &dirPath, int dateAdded, std::error_code &ec) const noexcept override;

    const fs::path &GetNotAuthDir();

    size_t GetFileExpirationDate();

    const std::vector<fs::path> &GetSortDirs();

private:
    static const size_t fileExpirationDateDefault = 10u;
    fs::path notAuthDir;
    const size_t fileExpirationDate;
    std::vector<fs::path> sortDirs;
};

class FsSubWorkerAuthUsr : public IFsSubWorker {
public:
    explicit FsSubWorkerAuthUsr(const fs::path &usrsDir) : usrsDir(usrsDir) {}

    explicit FsSubWorkerAuthUsr(fs::path &&usrsDir) : usrsDir(std::move(usrsDir)) {}

    FsSubWorkerAuthUsr(const FsSubWorkerAuthUsr &src) : usrsDir(src.usrsDir) {}

    FsSubWorkerAuthUsr(FsSubWorkerAuthUsr &&src) : usrsDir(std::move(src.usrsDir)) {}

    FsSubWorkerAuthUsr &operator=(const FsSubWorkerAuthUsr &);

    FsSubWorkerAuthUsr &operator=(FsSubWorkerAuthUsr &&);

    ~FsSubWorkerAuthUsr() noexcept = default;

    virtual bool MoveRoot(const fs::path &newUsrsDir, int, std::error_code &ec) noexcept override;

    bool MoveFile(const fs::path &filePath, int userId, std::error_code &ec) const noexcept override;

    std::ifstream GetFile(const fs::path &filePath, int userId) const noexcept override;

    bool RemoveFile(const fs::path &path, int userId, std::error_code &ec) const noexcept override;

    bool CreateDir(const fs::path &dirPath, int userId, std::error_code &ec) const noexcept override;

    bool RemoveDir(const fs::path &dirPath, int userId, std::error_code &ec) const noexcept override;

    const fs::path &GetUsrsDir();

private:
    fs::path usrsDir;
};

class FsSubWorkerGroup : public IFsSubWorker {
public:
    explicit FsSubWorkerGroup(const fs::path &groupDir) : groupsDir(groupDir) {}

    explicit FsSubWorkerGroup(fs::path &&groupDir) : groupsDir(std::move(groupDir)) {}

    FsSubWorkerGroup(const FsSubWorkerGroup &src) : groupsDir(src.groupsDir) {}

    FsSubWorkerGroup(FsSubWorkerGroup &&src) : groupsDir(std::move(src.groupsDir)) {}

    FsSubWorkerGroup &operator=(const FsSubWorkerGroup &);

    FsSubWorkerGroup &operator=(FsSubWorkerGroup &&);

    ~FsSubWorkerGroup() noexcept = default;

    virtual bool MoveRoot(const fs::path &newGroupsDir, int, std::error_code &ec) noexcept override;

    bool MoveFile(const fs::path &filePath, int groupId, std::error_code &ec) const noexcept override;

    std::ifstream GetFile(const fs::path &filePath, int groupId) const noexcept override;

    bool RemoveFile(const fs::path &path, int groupId, std::error_code &ec) const noexcept override;

    bool CreateDir(const fs::path &dirPath, int groupId, std::error_code &ec) const noexcept override;

    bool RemoveDir(const fs::path &dirPath, int groupId, std::error_code &ec) const noexcept override;

    const fs::path &GetGroupsDir();

private:
    fs::path groupsDir;
};

class FsWorker {
private:
    fs::path root;
public:
    explicit FsWorker(const fs::path &_root) : root(_root), notAuthUsr(root / notAuthUsrsDirDefault),
                                               authUsr(root / authUsrsDirDefault),
                                               group(root / groupsDirDefault) {}

    explicit FsWorker(fs::path &&_root) : root(std::move(_root)), notAuthUsr(root / notAuthUsrsDirDefault),
                                          authUsr(root / authUsrsDirDefault),
                                          group(root / groupsDirDefault) {}

    FsWorker(const FsWorker &src) : root(src.root), notAuthUsr(src.notAuthUsr),
                                    authUsr(src.authUsr),
                                    group(src.group) {}

    FsWorker(FsWorker &&src) : root(std::move(src.root)), notAuthUsr(std::move(src.notAuthUsr)),
                               authUsr(std::move(src.authUsr)),
                               group(std::move(src.group)) {}

    FsWorker &operator=(const FsWorker &);

    FsWorker &operator=(FsWorker &&);

    ~FsWorker() = default;

    static const fs::path notAuthUsrsDirDefault;
    static const fs::path authUsrsDirDefault;
    static const fs::path groupsDirDefault;
    FsSubWorkerNotAuthUsr notAuthUsr;
    FsSubWorkerAuthUsr authUsr;
    FsSubWorkerGroup group;
};

#endif //VVTI_WORKING_WITH_FILESYSTEM_H
