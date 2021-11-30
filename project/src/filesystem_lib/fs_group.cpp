// Copyright 2021 nat-s.skv@mail.ru

#include <vector>
#include <filesystem_lib.hpp>
#include <fstream>

FsGroup::FsGroup(const fs::path &groups_dir): root_path(groups_dir) {}

FsGroup::FsGroup(fs::path &&groups_dir) : root_path(std::move(groups_dir)) {}

FsGroup::FsGroup(const FsGroup&src) : root_path(src.root_path) {}

FsGroup::FsGroup(FsGroup&&src) noexcept: root_path(std::move(src.root_path)) {}

bool FsGroup::move_root(const fs::path &new_group_dir) noexcept {
    return false;
}

bool FsGroup::move_file(const fs::path &src_path, const fs::path &dst_path, std::string group_id) const noexcept {
    return false;
}

bool FsGroup::add(std::string group_id) const noexcept {
    return false;
}

std::ifstream FsGroup::get_file(const fs::path &file_path, std::string group_id) const noexcept {
    return {};
}

bool FsGroup::remove(const fs::path &path, std::string group_id) const noexcept {
    return false;
}

bool FsGroup::make_subdir(const fs::path &dir_path, std::string group_id) const noexcept {
    return false;
}

const fs::path &FsGroup::get_root() const noexcept {
    return root_path;
}

void FsGroup::reset_error_code() noexcept {
}

bool FsGroup::operator==(const FsGroup&rhs) const {
    return false;
}


