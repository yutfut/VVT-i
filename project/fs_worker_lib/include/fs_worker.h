// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_WORKING_WITH_FILESYSTEM_H
#define VVTI_WORKING_WITH_FILESYSTEM_H

#include <string>
#include <filesystem>
#include <vector>
#include <fs_not_auth_usr.h>
 #include <fs_auth_usr.h>

class FsWorker {
private:
    std::filesystem::path root;
public:

    explicit FsWorker(const std::filesystem::path &_root = root_path_default);
    explicit FsWorker(std::filesystem::path &&_root);

    FsWorker(const FsWorker &src);

    FsWorker(FsWorker &&src) noexcept ;

    FsWorker &operator=(const FsWorker &);

    bool operator==(const FsWorker &rhs) const;

    ~FsWorker() = default;

    [[nodiscard]] const std::filesystem::path &get_root_dir() const noexcept;

    std::error_code err_code;
    static const std::filesystem::path not_auth_usrs_dir_default;
    static const std::filesystem::path root_path_default;
    FsWorkerNotAuthUsr not_auth_usr;

    static const std::filesystem::path auth_usrs_dir_default;
    FsWorkerAuthUsr auth_usr;
};

#endif //VVTI_WORKING_WITH_FILESYSTEM_H