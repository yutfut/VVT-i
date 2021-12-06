// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_WORKING_WITH_FILESYSTEM_H
#define VVTI_WORKING_WITH_FILESYSTEM_H

#include <string>
#include <filesystem>
#include <vector>
#include <fs_not_auth_usr.h>
// #include <fs_auth_usr.h>
// #include <fs_group.h>
namespace fs = std::filesystem;

class FsWorker {
private:
    fs::path root;
public:
    explicit FsWorker(const fs::path &_root);

    explicit FsWorker(fs::path &&_root);

    FsWorker(const FsWorker &src);

    FsWorker(FsWorker &&src) ;

    FsWorker &operator=(const FsWorker &);

    bool operator==(const FsWorker &rhs) const;

    ~FsWorker() = default;

    const fs::path &get_root_dir() const noexcept;

    void reset_error_code() noexcept;

    std::error_code errc;
    static const fs::path not_auth_usrs_dir_default;
    static const fs::path auth_usrs_dir_default;
    static const fs::path groups_dir_default;
    FsNotAuthUsr not_auth_usr;
    //FsAuthUsr auth_usr;
    //FsGroup group;
};

#endif //VVTI_WORKING_WITH_FILESYSTEM_H