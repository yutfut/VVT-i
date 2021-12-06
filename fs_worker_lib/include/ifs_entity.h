// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_IFS_ENTITY_H
#define VVTI_IFS_ENTITY_H

#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class IFsEntity {

public:
    virtual bool move_root(const fs::path &new_user_dir) noexcept = 0;

    virtual bool
    move_file(const fs::path &src_path, const fs::path &dst_path, std::string entity_id) const noexcept = 0;

    virtual bool add(std::string entity_id) const noexcept = 0;

    [[nodiscard]] virtual std::ifstream get_file(const fs::path &file_path, std::string entity_id) const noexcept = 0;

    virtual bool remove(const fs::path &path, std::string entity_id) const noexcept = 0;

    virtual bool make_subdir(const fs::path &dir_path, std::string entity_id) const noexcept = 0;

    [[nodiscard]] virtual const fs::path &get_root() const noexcept = 0;

    virtual void reset_error_code() noexcept = 0;

};

#endif //VVTI_IFS_ENTITY_H
