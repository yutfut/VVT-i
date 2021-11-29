// Copyright 2021 nat-s.skv@mail.ru

#include <string>
#include <filesystem>
#include <vector>
#include <working_with_fs_lib.hpp>
#include <fstream>

FsSubWorkerGroup::FsSubWorkerGroup(const fs::path &groups_dir): root_path(groups_dir) {}

FsSubWorkerGroup::FsSubWorkerGroup(fs::path &&groups_dir) : root_path(std::move(groups_dir)) {}

FsSubWorkerGroup::FsSubWorkerGroup(const FsSubWorkerGroup&src) : root_path(src.root_path) {}

FsSubWorkerGroup::FsSubWorkerGroup(FsSubWorkerGroup&&src) noexcept: root_path(std::move(src.root_path)) {}

bool FsSubWorkerGroup::move_root(const fs::path &new_group_dir) noexcept {
    return false;
}

bool FsSubWorkerGroup::move_file_to_group_dir(const fs::path &src_path, const fs::path &dst_path, int group_id) const noexcept {
    return false;
}

bool FsSubWorkerGroup::add_group(int group_id) const noexcept {
    return false;
}

std::ifstream FsSubWorkerGroup::get_group_file(const fs::path &file_path, int group_id) const noexcept {
    return {};
}

bool FsSubWorkerGroup::remove_group_file(const fs::path &file_path, int group_id) const noexcept {
    return false;
}

bool FsSubWorkerGroup::make_group_subdir(const fs::path &dir_path, int group_id) const noexcept {
    return false;
}

bool FsSubWorkerGroup::remove_dir(const fs::path &dir_path, int group_id) const noexcept {
    return false;
}

const fs::path &FsSubWorkerGroup::get_root_path() const noexcept {
    return root_path;
}

void FsSubWorkerGroup::reset_error_code() noexcept {
}

bool FsSubWorkerGroup::operator==(const FsSubWorkerGroup&rhs) const {
    return false;
}

std::error_code err_code;

