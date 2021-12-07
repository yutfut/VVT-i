// Copyright 2021 nat-s.skv@mail.ru

#include <string>
#include <filesystem>
#include <vector>
#include "fs_worker.h"
#include <fstream>
#include "fs_auth_usr.h"

FsWorkerAuthUsr::FsWorkerAuthUsr(const fs::path &user_dir) : root_path(user_dir) {}

FsWorkerAuthUsr::FsWorkerAuthUsr(fs::path &&user_dir) : root_path(std::move(user_dir)) {}

FsWorkerAuthUsr::FsWorkerAuthUsr(const FsWorkerAuthUsr &src) : root_path(src.root_path) {}

FsWorkerAuthUsr::FsWorkerAuthUsr(FsWorkerAuthUsr &&src) noexcept: root_path(std::move(src.root_path)) {}

bool FsWorkerAuthUsr::move_root(const fs::path &new_user_dir) noexcept {
    return false;
}

bool FsWorkerAuthUsr::move_file(const fs::path &src_path, const fs::path &dst_path,
                                std::string user_id) const noexcept  {
    return false;
}

bool FsWorkerAuthUsr::add(std::string user_id) const noexcept {
    return false;
}

std::ifstream FsWorkerAuthUsr::get_file(const fs::path &file_path, std::string user_id) const noexcept {
    return {};
}

bool FsWorkerAuthUsr::remove(const fs::path &path, std::string user_id) const noexcept {
    return false;
}

bool FsWorkerAuthUsr::make_subdir(const fs::path &dir_path, std::string user_id) const noexcept {
    return false;
}

const fs::path &FsWorkerAuthUsr::get_root() const noexcept {
    return root_path;
}

void FsWorkerAuthUsr::reset_error_code() noexcept {
}

bool FsWorkerAuthUsr::operator==(const FsWorkerAuthUsr &rhs) const {
    return false;
}


