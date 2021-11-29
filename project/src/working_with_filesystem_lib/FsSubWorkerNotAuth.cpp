#include <string>
#include <filesystem>
#include <vector>
#include <working_with_fs_lib.hpp>
#include <fstream>

FsSubWorkerNotAuthUsr::FsSubWorkerNotAuthUsr(const fs::path &not_auth_dir,
                                             size_t file_expiration_date)
        : root_path(not_auth_dir), file_expiration_date(file_expiration_date), sort_dirs{file_expiration_date} {}


FsSubWorkerNotAuthUsr::FsSubWorkerNotAuthUsr(fs::path &&not_auth_dir,
                                             size_t file_expiration_date)
        : root_path(std::move(not_auth_dir)), file_expiration_date(file_expiration_date),
          sort_dirs{file_expiration_date} {}

FsSubWorkerNotAuthUsr::FsSubWorkerNotAuthUsr(const FsSubWorkerNotAuthUsr &src) : root_path(src.root_path),
                                                                                 file_expiration_date(
                                                                                         src.file_expiration_date),
                                                                                 sort_dirs(src.sort_dirs) {}

FsSubWorkerNotAuthUsr::FsSubWorkerNotAuthUsr(FsSubWorkerNotAuthUsr &&src) noexcept: root_path(
        std::move(src.root_path)),
                                                                                    file_expiration_date(
                                                                                            src.file_expiration_date),
                                                                                    sort_dirs(
                                                                                            std::move(src.sort_dirs)) {}

bool FsSubWorkerNotAuthUsr::move_root(const fs::path &new_not_auth_dir) noexcept {
    return false;
}

bool FsSubWorkerNotAuthUsr::move_file_to_fs(const fs::path &src_path, const std::string &dst_name,
                                            const std::string &date_added) const noexcept {
    return false;
}

std::ifstream FsSubWorkerNotAuthUsr::get_file(const fs::path &file_path, const std::string &date_added) const noexcept {
    return {};
}

bool FsSubWorkerNotAuthUsr::create_day_dir(const std::string &date_added) const noexcept {
    return false;
}

bool FsSubWorkerNotAuthUsr::remove_expired_dirs() const noexcept {
    return false;
}

const fs::path &FsSubWorkerNotAuthUsr::get_root_path() const noexcept {
    return root_path;
}

size_t FsSubWorkerNotAuthUsr::get_file_expiration_date() {
    return 0ul;
}

const std::vector<fs::path> &FsSubWorkerNotAuthUsr::get_sort_dirs() {
    return sort_dirs;
}

void FsSubWorkerNotAuthUsr::reset_error_code() noexcept {
}

void FsSubWorkerNotAuthUsr::set_file_expiration_date(size_t new_expiration_date) {

}

bool FsSubWorkerNotAuthUsr::operator==(const FsSubWorkerNotAuthUsr &rhs) const {
    return false;
}

/*
std::error_code err_code;

static const size_t file_expiration_date_default = 10u;
fs::path root_path;
const size_t file_expiration_date;
std::vector<fs::path> sort_dirs;
 */