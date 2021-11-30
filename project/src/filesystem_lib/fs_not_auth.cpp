// Copyright 2021 nat-s.skv@mail.ru

#include <vector>
#include <filesystem_lib.hpp>
#include <fstream>

FsNotAuthUsr::FsNotAuthUsr(const fs::path &not_auth_dir,
                           size_t file_expiration_date)
        : root_path(not_auth_dir), file_expiration_date(file_expiration_date), sort_dirs{file_expiration_date} {}


FsNotAuthUsr::FsNotAuthUsr(fs::path &&not_auth_dir,
                           size_t file_expiration_date)
        : root_path(std::move(not_auth_dir)), file_expiration_date(file_expiration_date),
          sort_dirs{file_expiration_date} {}

FsNotAuthUsr::FsNotAuthUsr(const FsNotAuthUsr &src) : root_path(src.root_path),
                                                      file_expiration_date(src.file_expiration_date),
                                                      sort_dirs(src.sort_dirs) {}

FsNotAuthUsr::FsNotAuthUsr(FsNotAuthUsr &&src) noexcept: root_path(
        std::move(src.root_path)),
                                                         file_expiration_date(
                                                                 src.file_expiration_date),
                                                         sort_dirs(
                                                                 std::move(src.sort_dirs)) {}

bool FsNotAuthUsr::move_root(const fs::path &new_not_auth_dir) noexcept {
    return false;
}

bool FsNotAuthUsr::move_file_to_fs(const fs::path &src_path, const std::string &dst_name,
                                   const std::string &date_added) const noexcept {
    return false;
}

std::ifstream FsNotAuthUsr::get_file(const fs::path &file_path, const std::string &date_added) const noexcept {
    return {};
}

bool FsNotAuthUsr::create_day_dir(const std::string &date_added) const noexcept {
    return false;
}

bool FsNotAuthUsr::remove_expired_dirs() const noexcept {
    return false;
}

const fs::path &FsNotAuthUsr::get_root() const noexcept {
    return root_path;
}

size_t FsNotAuthUsr::get_file_expiration_date() {
    return 0ul;
}

const std::vector<fs::path> &FsNotAuthUsr::get_sort_dirs() {
    return sort_dirs;
}

void FsNotAuthUsr::reset_error_code() noexcept {
}

void FsNotAuthUsr::set_file_expiration_date(size_t new_expiration_date) {

}

bool FsNotAuthUsr::operator==(const FsNotAuthUsr &rhs) const {
    return false;
}
