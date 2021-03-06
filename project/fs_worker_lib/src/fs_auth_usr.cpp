// Copyright 2021 nat-s.skv@mail.ru

#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include "fs_auth_usr.h"

namespace fs = std::filesystem;

FsWorkerAuthUsr::FsWorkerAuthUsr(const fs::path &_root_path) : root_path(_root_path) {
    fs::create_directory(root_path, err_code);
    if (bool(err_code)) {
        root_path = "";
    }
}

FsWorkerAuthUsr::FsWorkerAuthUsr(fs::path &&_root_path) : root_path(std::move(_root_path)) {
    fs::create_directory(root_path, err_code);
    if (bool(err_code)) {
        root_path = "";
    }
}

FsWorkerAuthUsr::FsWorkerAuthUsr(const FsWorkerAuthUsr &src) : root_path(src.root_path) {}

FsWorkerAuthUsr::FsWorkerAuthUsr(FsWorkerAuthUsr &&src) noexcept: root_path(std::move(src.root_path)) {
    fs::create_directory(root_path, err_code);
    if (bool(err_code)) {
        root_path = "";
    }
}

bool FsWorkerAuthUsr::move_root(const fs::path &new_root_path) noexcept {
    err_code.clear();
    fs::rename(root_path, new_root_path, err_code);
    if (!bool(err_code)) {
        root_path = new_root_path;
    }
    return !bool(err_code);
}

bool FsWorkerAuthUsr::move_file(const fs::path &src_path, const fs::path &dst_path,
                                std::string user_id) noexcept {
    err_code.clear();
    fs::rename(src_path, root_path / user_id / dst_path.relative_path(), err_code);
    return !bool(err_code);
}

bool FsWorkerAuthUsr::write_to_file(const std::string &file_content, const fs::path &file_path,
                                std::string user_id) noexcept {
    err_code.clear();
    std::ofstream file{root_path / user_id / file_path.relative_path() , std::ios_base::binary};
    file << file_content;
    return file.good();
}

bool FsWorkerAuthUsr::add(std::string user_id) noexcept {
    err_code.clear();
    fs::create_directory(root_path / user_id, err_code);
    return !bool(err_code);
}

std::ifstream FsWorkerAuthUsr::get_file(const fs::path &file_path, std::string user_id) noexcept {
    return {root_path / user_id / file_path.relative_path(), std::ios_base::binary};
}

bool FsWorkerAuthUsr::remove(const fs::path &path, std::string user_id) noexcept {
    err_code.clear();
    fs::remove_all(root_path / user_id / path.relative_path(), err_code);
    return !bool(err_code);
}

bool FsWorkerAuthUsr::make_subdir(const fs::path &dir_path, std::string user_id) noexcept {
    err_code.clear();
    fs::create_directory(root_path / user_id / dir_path.relative_path(), err_code);
    return !bool(err_code);
}

const fs::path &FsWorkerAuthUsr::get_root() const noexcept {
    return root_path;
}

bool FsWorkerAuthUsr::operator==(const FsWorkerAuthUsr &rhs) const {
    return root_path == rhs.root_path;
}
