// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_FS_GROUP_H
#define VVTI_FS_GROUP_H

#include <string>
#include <filesystem>
#include <vector>
#include <ifs_entity.h>

namespace fs = std::filesystem;

class FsGroup : public IFsEntity {
public:
    explicit FsGroup(const fs::path &groups_dir);

    explicit FsGroup(fs::path &&groups_dir);

    FsGroup(const FsGroup &src);

    FsGroup(FsGroup &&src) noexcept;

    ~FsGroup() noexcept = default;

    bool add(std::string group_id) const noexcept override;

    bool move_root(const fs::path &new_groups_dir) noexcept override;

    bool move_file(const fs::path &src_path, const fs::path &dst_path, std::string group_id) const noexcept override;

    [[nodiscard]] std::ifstream get_file(const fs::path &file_path, std::string group_id) const noexcept override;

    bool remove(const fs::path &file_path, std::string group_id) const noexcept override;

    bool make_subdir(const fs::path &dir_path, std::string group_id) const noexcept override;

    [[nodiscard]] const fs::path &get_root() const noexcept override;

    void reset_error_code() noexcept override;

    bool operator==(const FsGroup &rhs) const;

    std::error_code err_code;

private:
    fs::path root_path;
};

#endif //VVTI_FS_GROUP_H
