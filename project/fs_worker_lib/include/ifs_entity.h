// Copyright 2021 nat-s.skv@mail.ru

#ifndef VVTI_IFS_ENTITY_H
#define VVTI_IFS_ENTITY_H

#include <string>
#include <filesystem>
#include <vector>


class IFsWorkerEntity {

public:
    virtual bool move_root(const std::filesystem::path &new_user_dir) noexcept = 0;

    virtual bool
    move_file(const std::filesystem::path &src_path, const std::filesystem::path &dst_path, std::string entity_id)  noexcept = 0;

    virtual bool add(std::string entity_id)  noexcept = 0;

    [[nodiscard]] virtual std::ifstream get_file(const std::filesystem::path &file_path, std::string entity_id)  noexcept = 0;

    virtual bool remove(const std::filesystem::path &path, std::string entity_id)  noexcept = 0;

    virtual bool make_subdir(const std::filesystem::path &dir_path, std::string entity_id)  noexcept = 0;

    [[nodiscard]] virtual const std::filesystem::path &get_root() const noexcept = 0;

};

#endif //VVTI_IFS_ENTITY_H
