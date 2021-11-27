// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_WORKING_WITH_FILESYSTEM_H
#define VVTI_WORKING_WITH_FILESYSTEM_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FsSubWorkerNotAuthUsr {
public:
    explicit FsSubWorkerNotAuthUsr(const fs::path &not_auth_dir,
                                   size_t file_expiration_date = file_expiration_date_default)
            : not_auth_dir(not_auth_dir), file_expiration_date(file_expiration_date), sort_dirs{file_expiration_date} {}

    explicit FsSubWorkerNotAuthUsr(fs::path &&not_auth_dir, size_t file_expiration_date = file_expiration_date_default)
            : not_auth_dir(
            std::move(not_auth_dir)), file_expiration_date(file_expiration_date), sort_dirs{file_expiration_date} {}

    FsSubWorkerNotAuthUsr(const FsSubWorkerNotAuthUsr &src) : not_auth_dir(src.not_auth_dir),
                                                              file_expiration_date(src.file_expiration_date),
                                                              sort_dirs(src.sort_dirs) {}

    FsSubWorkerNotAuthUsr(FsSubWorkerNotAuthUsr &&src) : not_auth_dir(std::move(src.not_auth_dir)),
                                                         file_expiration_date(src.file_expiration_date),
                                                         sort_dirs(std::move(src.sort_dirs)) {}

    ~FsSubWorkerNotAuthUsr() noexcept = default;

    bool move_root(const fs::path &new_not_auth_dir) noexcept;

    bool move_file_to_fs(const fs::path &src_path, const std::string &dst_name, const std::string &date_added) const noexcept;

    std::ifstream get_file(const fs::path &file_path, const std::string &date_added) const noexcept;

    bool create_day_dir(const std::string &date_added) const noexcept;

    bool remove_expired_dirs() const noexcept;

    const fs::path &get_root_path() const noexcept;

    size_t get_file_expiration_date();

    const std::vector<fs::path> &get_sort_dirs();

    void reset_error_code() noexcept;

    void set_expiration_date(size_t new_expiration_date);

    bool operator==(const FsSubWorkerNotAuthUsr &rhs) const;

    std::error_code err_code;

private:
    static const size_t file_expiration_date_default = 10u;
    fs::path not_auth_dir;
    const size_t file_expiration_date;
    std::vector<fs::path> sort_dirs;
};

class FsSubWorkerAuthUsr {
public:
    explicit FsSubWorkerAuthUsr(const fs::path &usrs_dir) : usrs_dir(usrs_dir) {}

    explicit FsSubWorkerAuthUsr(fs::path &&usrs_dir) : usrs_dir(std::move(usrs_dir)) {}

    FsSubWorkerAuthUsr(const FsSubWorkerAuthUsr &src) : usrs_dir(src.usrs_dir) {}

    FsSubWorkerAuthUsr(FsSubWorkerAuthUsr &&src) : usrs_dir(std::move(src.usrs_dir)) {}

    ~FsSubWorkerAuthUsr() noexcept = default;

    bool move_root(const fs::path &new_usrs_dir) noexcept;

    bool move(const fs::path &path_src, const fs::path &path_dst, int user_id) const noexcept;

    bool add_user(int user_id) const noexcept;

    std::ifstream get_file(const fs::path &file_path, int user_id) const noexcept;

    bool remove_file(const fs::path &file_path, int user_id) const noexcept;

    bool create_dir(const fs::path &dir_path, int user_id) const noexcept;

    bool remove_dir(const fs::path &dir_path, int user_id) const noexcept;

    const fs::path &get_root_path() const noexcept;

    void reset_error_code() noexcept;

    bool operator==(const FsSubWorkerAuthUsr &rhs) const;

    std::error_code err_code;

private:
    fs::path usrs_dir;
};

class FsSubWorkerGroup {
public:
    explicit FsSubWorkerGroup(const fs::path &groups_dir) : groups_dir(groups_dir) {}

    explicit FsSubWorkerGroup(fs::path &&groups_dir) : groups_dir(std::move(groups_dir)) {}

    FsSubWorkerGroup(const FsSubWorkerGroup &src) : groups_dir(src.groups_dir) {}

    FsSubWorkerGroup(FsSubWorkerGroup &&src) : groups_dir(std::move(src.groups_dir)) {}

    ~FsSubWorkerGroup() noexcept = default;

    bool add_group(int group_id);

    bool move_root(const fs::path &new_groups_dir) noexcept;

    bool move(const fs::path &path_src, const fs::path &path_dst, int group_id) const noexcept;

    std::ifstream get_file(const fs::path &file_path, int group_id) const noexcept;

    bool remove_file(const fs::path &file_path, int group_id) const noexcept;

    bool create_dir(const fs::path &dir_path, int group_id) const noexcept;

    bool remove_dir(const fs::path &dir_path, int group_id) const noexcept;

    const fs::path &get_root_path() const noexcept;

    void reset_error_code() noexcept;

    bool operator==(const FsSubWorkerGroup &rhs) const;

    std::error_code err_code;


private:
    fs::path groups_dir;
};

class FsWorker {
private:
    fs::path root;
public:
    explicit FsWorker(const fs::path &_root) : root(_root), not_auth_usr(root / not_auth_usrs_dir_default),
                                               auth_usr(root / auth_usrs_dir_default),
                                               group(root / groups_dir_default) {}

    explicit FsWorker(fs::path &&_root) : root(std::move(_root)), not_auth_usr(root / not_auth_usrs_dir_default),
                                          auth_usr(root / auth_usrs_dir_default),
                                          group(root / groups_dir_default) {}

    FsWorker(const FsWorker &src) : root(src.root), not_auth_usr(src.not_auth_usr),
                                    auth_usr(src.auth_usr),
                                    group(src.group) {}

    FsWorker(FsWorker &&src) : root(std::move(src.root)), not_auth_usr(std::move(src.not_auth_usr)),
                               auth_usr(std::move(src.auth_usr)),
                               group(std::move(src.group)) {}

    FsWorker &operator=(const FsWorker &);

    bool operator==(const FsWorker &rhs) const;

    ~FsWorker() = default;

    const fs::path &get_root_dir() const noexcept;

    void reset_error_code() noexcept;

    std::error_code errc;
    static const fs::path not_auth_usrs_dir_default;
    static const fs::path auth_usrs_dir_default;
    static const fs::path groups_dir_default;
    FsSubWorkerNotAuthUsr not_auth_usr;
    FsSubWorkerAuthUsr auth_usr;
    FsSubWorkerGroup group;
};

#endif //VVTI_WORKING_WITH_FILESYSTEM_H