// Copyright 2021 nat-s.skv@mail.ru

#include <vector>
#include <fs_worker.h>
#include <fstream>
#include <fs_group.h>

FsWorkerGroup::FsWorkerGroup(const fs::path &groups_dir): root_path(groups_dir) {}

FsWorkerGroup::FsWorkerGroup(fs::path &&groups_dir) : root_path(std::move(groups_dir)) {}

FsWorkerGroup::FsWorkerGroup(const FsWorkerGroup&src) : root_path(src.root_path) {}

FsWorkerGroup::FsWorkerGroup(FsWorkerGroup&&src) noexcept: root_path(std::move(src.root_path)) {}

bool FsWorkerGroup::move_root(const fs::path &new_group_dir) noexcept {
    return false;
}

bool FsWorkerGroup::move_file(const fs::path &src_path, const fs::path &dst_path, std::string group_id) const noexcept {
    return false;
}

bool FsWorkerGroup::add(std::string group_id) const noexcept {
    return false;
}

std::ifstream FsWorkerGroup::get_file(const fs::path &file_path, std::string group_id) const noexcept {
    return {};
}

bool FsWorkerGroup::remove(const fs::path &path, std::string group_id) const noexcept {
    return false;
}

bool FsWorkerGroup::make_subdir(const fs::path &dir_path, std::string group_id) const noexcept {
    return false;
}

const fs::path &FsWorkerGroup::get_root() const noexcept {
    return root_path;
}

void FsWorkerGroup::reset_error_code() noexcept {
}

bool FsWorkerGroup::operator==(const FsWorkerGroup&rhs) const {
    return false;
}


