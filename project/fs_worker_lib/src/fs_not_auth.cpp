// Copyright 2021 nat-s.skv@mail.ru

#include <vector>
#include <fstream>
#include <fs_not_auth_usr.h>

FsWorkerNotAuthUsr::FsWorkerNotAuthUsr(const fs::path &_root_path, size_t file_expiration_date)
        : root_path(_root_path), file_expiration_date(file_expiration_date) {
    fs::create_directory(root_path, err_code);
    if (static_cast<bool>(err_code)) {
        root_path = "";
    }
}


FsWorkerNotAuthUsr::FsWorkerNotAuthUsr(fs::path &&_root_path, size_t file_expiration_date)
        : root_path(std::move(_root_path)), file_expiration_date(file_expiration_date) {
    fs::create_directory(root_path, err_code);
    if (static_cast<bool>(err_code)) {
        root_path = "";
    }
}

FsWorkerNotAuthUsr::FsWorkerNotAuthUsr(const FsWorkerNotAuthUsr &src) : root_path(src.root_path),
                                                                        file_expiration_date(src.file_expiration_date),
                                                                        sort_dirs(src.sort_dirs) {}

FsWorkerNotAuthUsr::FsWorkerNotAuthUsr(FsWorkerNotAuthUsr &&src) noexcept: root_path(std::move(src.root_path)),
                                                                           file_expiration_date(src.file_expiration_date),
                                                                           sort_dirs(std::move(src.sort_dirs)) {}

bool FsWorkerNotAuthUsr::move_root(const fs::path &new_root_path) noexcept {
    err_code.clear();
    fs::rename(root_path, new_root_path, err_code);
    if (!static_cast<bool>(err_code)) {
        root_path = new_root_path;
    }
    return !static_cast<bool>(err_code);
}

bool FsWorkerNotAuthUsr::move_file_to_fs(const fs::path &src_path, const fs::path &dst_name,
                                         const fs::path &date_added) noexcept {
    err_code.clear();
    fs::rename(src_path, root_path / date_added / dst_name, err_code);
    return !static_cast<bool>(err_code);
}

std::ifstream FsWorkerNotAuthUsr::get_file(const fs::path &file_name, const fs::path &date_added) const noexcept {
    return {root_path / date_added / file_name, std::ios_base::binary};
}

bool FsWorkerNotAuthUsr::create_day_dir(const fs::path &date_added) noexcept {
    err_code.clear();
    fs::create_directory(root_path / date_added, err_code);
    if (!static_cast<bool>(err_code)) {
        sort_dirs.emplace_back(date_added);
    }
    return !static_cast<bool>(err_code);
}

bool FsWorkerNotAuthUsr::remove_expired_dirs() noexcept {
    err_code.clear();
    std::for_each(sort_dirs.begin() + static_cast<int>(file_expiration_date), sort_dirs.end(),
                  [this](const auto &expired_dir) { fs::remove_all(root_path / expired_dir, err_code); });
    sort_dirs.erase(sort_dirs.begin() + static_cast<int>(file_expiration_date), sort_dirs.end());
    return !static_cast<bool>(err_code);
}

const fs::path &FsWorkerNotAuthUsr::get_root() const noexcept {
    return root_path;
}

size_t FsWorkerNotAuthUsr::get_file_expiration_date() const noexcept {
    return file_expiration_date;
}

const std::vector<fs::path> &FsWorkerNotAuthUsr::get_sort_dirs() const noexcept {
    return sort_dirs;
}

void FsWorkerNotAuthUsr::set_file_expiration_date(size_t new_expiration_date) noexcept {
    file_expiration_date = new_expiration_date;
}

bool FsWorkerNotAuthUsr::operator==(const FsWorkerNotAuthUsr &rhs) const {
    return (file_expiration_date == rhs.file_expiration_date) && (root_path == rhs.root_path) &&
           (sort_dirs == rhs.sort_dirs);
}
