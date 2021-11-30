// Copyright 2021 nat-s.skv@mail.ru

#include <string>
#include <filesystem>
#include <vector>
#include <filesystem_lib.hpp>
#include <fstream>

FsAuthUsr::FsAuthUsr(const fs::path &user_dir) : root_path(user_dir) {}

FsAuthUsr::FsAuthUsr(fs::path &&user_dir) : root_path(std::move(user_dir)) {}

FsAuthUsr::FsAuthUsr(const FsAuthUsr &src) : root_path(src.root_path) {}

FsAuthUsr::FsAuthUsr(FsAuthUsr &&src) noexcept: root_path(std::move(src.root_path)) {}

bool FsAuthUsr::move_root(const fs::path &new_user_dir) noexcept {
    return false;
}

bool FsAuthUsr::move_file(const fs::path &src_path, const fs::path &dst_path,
                                      std::string user_id) const noexcept  {
    return false;
}

bool FsAuthUsr::add(std::string user_id) const noexcept {
    return false;
}

std::ifstream FsAuthUsr::get_file(const fs::path &file_path, std::string user_id) const noexcept {
    return {};
}

bool FsAuthUsr::remove(const fs::path &path, std::string user_id) const noexcept {
    return false;
}

bool FsAuthUsr::make_subdir(const fs::path &dir_path, std::string user_id) const noexcept {
    return false;
}

const fs::path &FsAuthUsr::get_root() const noexcept {
    return root_path;
}

void FsAuthUsr::reset_error_code() noexcept {
}

bool FsAuthUsr::operator==(const FsAuthUsr &rhs) const {
    return false;
}


