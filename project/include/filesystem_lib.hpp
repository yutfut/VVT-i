// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_WORKING_WITH_FILESYSTEM_H
#define VVTI_WORKING_WITH_FILESYSTEM_H

#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class FsNotAuthUsr {
public:
    explicit FsNotAuthUsr(const fs::path &not_auth_dir,
                          size_t file_expiration_date = file_expiration_date_default);

    explicit FsNotAuthUsr(fs::path &&not_auth_dir, size_t file_expiration_date = file_expiration_date_default);

    FsNotAuthUsr(const FsNotAuthUsr &src);

    FsNotAuthUsr(FsNotAuthUsr &&src) noexcept;

    ~FsNotAuthUsr() noexcept = default;

    bool move_root(const fs::path &new_not_auth_dir) noexcept;

    [[nodiscard]] bool move_file_to_fs(const fs::path &src_path, const std::string &dst_name,
                                       const std::string &date_added) const noexcept;

    [[nodiscard]] std::ifstream get_file(const fs::path &file_path, const std::string &date_added) const noexcept;

    bool create_day_dir(const std::string &date_added) const noexcept;

    bool remove_expired_dirs() const noexcept;

    [[nodiscard]] const fs::path &get_root() const noexcept;

    size_t get_file_expiration_date();

    const std::vector<fs::path> &get_sort_dirs();

    void reset_error_code() noexcept;

    void set_file_expiration_date(size_t new_expiration_date);

    bool operator==(const FsNotAuthUsr &rhs) const;

    std::error_code err_code;

private:
    static const size_t file_expiration_date_default = 10u;
    fs::path root_path;
    const size_t file_expiration_date;
    std::vector<fs::path> sort_dirs;
};

class IFsEntity {

public:
    virtual bool move_root(const fs::path &new_user_dir) noexcept = 0;

    virtual bool
    move_file(const fs::path &src_path, const fs::path &dst_path, std::string entity_id) const noexcept = 0;

    virtual bool add(std::string entity_id) const noexcept = 0;

    [[nodiscard]] virtual std::ifstream get_file(const fs::path &file_path, std::string entity_id) const noexcept = 0;

    virtual bool remove(const fs::path &path, std::string entity_id) const noexcept = 0;

    virtual bool make_subdir(const fs::path &dir_path, std::string entity_id) const noexcept = 0;

    [[nodiscard]] virtual const fs::path &get_root() const noexcept = 0;

    virtual void reset_error_code() noexcept = 0;

};

class FsAuthUsr : public IFsEntity {
public:
    explicit FsAuthUsr(const fs::path &user_dir);

    explicit FsAuthUsr(fs::path &&user_dir);

    FsAuthUsr(const FsAuthUsr &src);

    FsAuthUsr(FsAuthUsr &&src) noexcept;

    ~FsAuthUsr() noexcept = default;

    bool move_root(const fs::path &new_user_dir) noexcept override;

    bool move_file(const fs::path &src_path, const fs::path &dst_path, std::string user_id) const noexcept override;

    bool add(std::string user_id) const noexcept override;

    [[nodiscard]] std::ifstream get_file(const fs::path &file_path, std::string user_id) const noexcept override;

    bool remove(const fs::path &path, std::string user_id) const noexcept override;

    bool make_subdir(const fs::path &dir_path, std::string user_id) const noexcept override;

    [[nodiscard]] const fs::path &get_root() const noexcept override;

    void reset_error_code() noexcept override;

    bool operator==(const FsAuthUsr &rhs) const;

    std::error_code err_code;

private:
    fs::path root_path;
};

class FsGroup : public IFsEntity {
public:
    explicit FsGroup(const fs::path &groups_dir);

    explicit FsGroup(fs::path &&groups_dir);

    FsGroup(const FsGroup &src);

    FsGroup(FsGroup &&src) noexcept;

    ~FsGroup() noexcept = default;

    bool add(std::string group_id) const noexcept override;

    bool move_root(const fs::path &new_groups_dir) noexcept override;

    bool move_file(const fs::path &src_path, const fs::path &dst_path, std::string group_id) const noexcept override;

    [[nodiscard]] std::ifstream get_file(const fs::path &file_path, std::string group_id) const noexcept override;

    bool remove(const fs::path &file_path, std::string group_id) const noexcept override;

    bool make_subdir(const fs::path &dir_path, std::string group_id) const noexcept override;

    [[nodiscard]] const fs::path &get_root() const noexcept override;

    void reset_error_code() noexcept override;

    bool operator==(const FsGroup &rhs) const;

    std::error_code err_code;

private:
    fs::path root_path;
};

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
    FsAuthUsr auth_usr;
    FsGroup group;
};

#endif //VVTI_WORKING_WITH_FILESYSTEM_H