// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_FS_NOT_AUTH_USR_H
#define VVTI_FS_NOT_AUTH_USR_H

#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class FsWorkerNotAuthUsr {
public:
    explicit FsWorkerNotAuthUsr(const fs::path &root_path,
                                size_t file_expiration_date = file_expiration_date_default);

    explicit FsWorkerNotAuthUsr(fs::path &&root_path, size_t file_expiration_date = file_expiration_date_default);

    FsWorkerNotAuthUsr(const FsWorkerNotAuthUsr &src);

    FsWorkerNotAuthUsr(FsWorkerNotAuthUsr &&src) noexcept;

    ~FsWorkerNotAuthUsr() noexcept = default;

    bool move_root(const fs::path &new_root_path) noexcept;

    [[nodiscard]] bool move_file_to_fs(const fs::path &src_path, const fs::path &dst_name,
                                       const fs::path &date_added) noexcept;

    [[nodiscard]] std::ifstream get_file(const fs::path &file_name, const fs::path &date_added) const noexcept;

    bool create_day_dir(const fs::path &date_added) noexcept;

    bool remove_expired_dirs() noexcept;

    [[nodiscard]] const fs::path &get_root() const noexcept;

    size_t get_file_expiration_date() const noexcept;

    const std::vector<fs::path> &get_sort_dirs() const noexcept;

    void set_file_expiration_date(size_t new_expiration_date) noexcept;

    bool operator==(const FsWorkerNotAuthUsr &rhs) const;

    std::error_code err_code;

private:
    static const size_t file_expiration_date_default = 10u;
    fs::path root_path;
    size_t file_expiration_date;
    std::vector<fs::path> sort_dirs;
};

#endif //VVTI_FS_NOT_AUTH_USR_H
