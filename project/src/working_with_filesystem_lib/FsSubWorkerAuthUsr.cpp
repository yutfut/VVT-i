#include <string>
#include <filesystem>
#include <vector>
#include <working_with_fs_lib.hpp>
#include <fstream>

FsSubWorkerAuthUsr::FsSubWorkerAuthUsr(const fs::path &usrs_dir) : root_path(usrs_dir) {}

FsSubWorkerAuthUsr::FsSubWorkerAuthUsr(fs::path &&usrs_dir) : root_path(std::move(usrs_dir)) {}

FsSubWorkerAuthUsr::FsSubWorkerAuthUsr(const FsSubWorkerAuthUsr &src) : root_path(src.root_path) {}

FsSubWorkerAuthUsr::FsSubWorkerAuthUsr(FsSubWorkerAuthUsr &&src) noexcept: root_path(std::move(src.root_path)) {}

bool FsSubWorkerAuthUsr::move_root(const fs::path &new_usrs_dir) noexcept {
    return false;
}

bool FsSubWorkerAuthUsr::move_file_to_user_dir(const fs::path &src_path, const fs::path &dst_path,
                                               int user_id) const noexcept {
    return false;
}

bool FsSubWorkerAuthUsr::add_user(int user_id) const noexcept {
    return false;
}

std::ifstream FsSubWorkerAuthUsr::get_user_file(const fs::path &file_path, int user_id) const noexcept {
    return {};
}

bool FsSubWorkerAuthUsr::remove_user_file(const fs::path &file_path, int user_id) const noexcept {
    return false;
}

bool FsSubWorkerAuthUsr::make_user_subdir(const fs::path &dir_path, int user_id) const noexcept {
    return false;
}

bool FsSubWorkerAuthUsr::remove_dir(const fs::path &dir_path, int user_id) const noexcept {
    return false;
}

const fs::path &FsSubWorkerAuthUsr::get_root_path() const noexcept {
    return root_path;
}

void FsSubWorkerAuthUsr::reset_error_code() noexcept {
}

bool FsSubWorkerAuthUsr::operator==(const FsSubWorkerAuthUsr &rhs) const {
    return false;
}

std::error_code err_code;

