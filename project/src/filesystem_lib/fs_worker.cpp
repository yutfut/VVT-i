// Copyright 2021 nat-s.skv@mail.ru
#include <fs_worker.hpp>


static const fs::path not_auth_usrs_dir_default;
static const fs::path auth_usrs_dir_default;
static const fs::path groups_dir_default;

FsWorker::FsWorker(const fs::path &_root) : root(_root), not_auth_usr(root / not_auth_usrs_dir_default) {}

FsWorker::FsWorker(fs::path &&_root) : root(std::move(_root)), not_auth_usr(root / not_auth_usrs_dir_default) {}

FsWorker::FsWorker(const FsWorker &src) : root(src.root), not_auth_usr(src.not_auth_usr) {}

FsWorker::FsWorker(FsWorker &&src) : root(std::move(src.root)), not_auth_usr(std::move(src.not_auth_usr)) {}

FsWorker &FsWorker::operator=(const FsWorker &) {}

bool FsWorker::operator==(const FsWorker &rhs) const {}

const fs::path &FsWorker::get_root_dir() const noexcept {}

void FsWorker::reset_error_code() noexcept {}
