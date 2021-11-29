// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_WORKING_WITH_FILESYSTEM_H
#define VVTI_WORKING_WITH_FILESYSTEM_H

#include <string>
#include <filesystem>
//TODO: IFsSubWorkerEntity

namespace fs = std::filesystem;

class FsSubWorkerNotAuthUsr {
public:
    explicit FsSubWorkerNotAuthUsr(const fs::path &not_auth_dir,
                                   size_t file_expiration_date = file_expiration_date_default);

    explicit FsSubWorkerNotAuthUsr(fs::path &&not_auth_dir, size_t file_expiration_date = file_expiration_date_default);

    FsSubWorkerNotAuthUsr(const FsSubWorkerNotAuthUsr &src);

    FsSubWorkerNotAuthUsr(FsSubWorkerNotAuthUsr &&src) noexcept;

    ~FsSubWorkerNotAuthUsr() noexcept = default;

    bool move_root(const fs::path &new_not_auth_dir) noexcept;

    [[nodiscard]] bool move_file_to_fs(const fs::path &src_path, const std::string &dst_name,
                                       const std::string &date_added) const noexcept;

     [[nodiscard]] std::ifstream get_file(const fs::path &file_path, const std::string &date_added) const noexcept;

     bool create_day_dir(const std::string &date_added) const noexcept;

     bool remove_expired_dirs() const noexcept;

    [[nodiscard]] const fs::path &get_root_path() const noexcept;

    size_t get_file_expiration_date();

    const std::vector<fs::path> &get_sort_dirs();

    void reset_error_code() noexcept;

    void set_file_expiration_date(size_t new_expiration_date);

    bool operator==(const FsSubWorkerNotAuthUsr &rhs) const;

    std::error_code err_code;

private:
    static const size_t file_expiration_date_default = 10u;
    fs::path root_path;
    const size_t file_expiration_date;
    std::vector<fs::path> sort_dirs;
};

class FsSubWorkerAuthUsr {
public:
    explicit FsSubWorkerAuthUsr(const fs::path &user_dir);

    explicit FsSubWorkerAuthUsr(fs::path &&user_dir);

    FsSubWorkerAuthUsr(const FsSubWorkerAuthUsr &src);

    FsSubWorkerAuthUsr(FsSubWorkerAuthUsr &&src) noexcept ;

    ~FsSubWorkerAuthUsr() noexcept = default;

    bool move_root(const fs::path &new_user_dir) noexcept;

    bool move_file_to_user_dir(const fs::path &src_path, const fs::path &dst_path, int user_id) const noexcept;

    bool add_user(int user_id) const noexcept;

    [[nodiscard]] std::ifstream get_user_file(const fs::path &file_path, int user_id) const noexcept;

    bool remove_user_file(const fs::path &file_path, int user_id) const noexcept;

    bool make_user_subdir(const fs::path &dir_path, int user_id) const noexcept;

    bool remove_dir(const fs::path &dir_path, int user_id) const noexcept;

    [[nodiscard]] const fs::path &get_root_path() const noexcept;

    void reset_error_code() noexcept;

    bool operator==(const FsSubWorkerAuthUsr &rhs) const;

    std::error_code err_code;

private:
    fs::path root_path;
};

class FsSubWorkerGroup {
public:
    explicit FsSubWorkerGroup(const fs::path &groups_dir) ;

    explicit FsSubWorkerGroup(fs::path &&groups_dir);

    FsSubWorkerGroup(const FsSubWorkerGroup &src);

    FsSubWorkerGroup(FsSubWorkerGroup &&src) noexcept ;

    ~FsSubWorkerGroup() noexcept = default;

    bool add_group(int group_id)const noexcept;

    bool move_root(const fs::path &new_groups_dir) noexcept;

    bool move_file_to_group_dir(const fs::path &src_path, const fs::path &dst_path, int group_id) const noexcept;

    [[nodiscard]] std::ifstream get_group_file(const fs::path &file_path, int group_id) const noexcept;

    bool remove_group_file(const fs::path &file_path, int group_id) const noexcept;

    bool make_group_subdir(const fs::path &dir_path, int group_id) const noexcept;

    bool remove_dir(const fs::path &dir_path, int group_id) const noexcept;

    [[nodiscard]] const fs::path &get_root_path() const noexcept;

    void reset_error_code() noexcept;

    bool operator==(const FsSubWorkerGroup &rhs) const;

    std::error_code err_code;


private:
    fs::path root_path;
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