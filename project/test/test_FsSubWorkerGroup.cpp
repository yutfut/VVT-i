// Copyright 2021 nat-s.skv@mail.ru

#include <gtest/gtest.h>
#include <working_with_fs_lib.hpp>
#include <fstream>

FsSubWorkerGroup create_worker_group(const fs::path &root_path) {
    FsSubWorkerGroup fs_worker(root_path);
    EXPECT_EQ(root_path, fs_worker.get_root_path());
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
    return fs_worker;
}

class FixtureCreateFsSubWorkerGroupInvalid : public ::testing::Test {
protected:
    static void set_up(fs::path &&root_path, std::errc &&exp_errc) {
        FsSubWorkerGroup fs_worker1(root_path);
        EXPECT_EQ(fs::path(""), fs_worker1.get_root_path());
        EXPECT_EQ(fs_worker1.err_code, exp_errc);

        FsSubWorkerGroup fs_worker2(std::move(root_path));
        EXPECT_EQ(fs_worker1, fs_worker2);
        EXPECT_EQ(fs_worker2.err_code, exp_errc);
    }
};

class FixtureFsSubWorkerGroupMoveRootInvalid : public ::testing::Test {
protected:
    static void set_up(fs::path &root_path, const fs::path &new_root_path, std::errc &&exp_errc) {
        FsSubWorkerGroup fs_worker(root_path);
        EXPECT_EQ(root_path, fs_worker.get_root_path());
        EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

        fs_worker.move_root(new_root_path);
        EXPECT_EQ(fs_worker.err_code, exp_errc);
        EXPECT_EQ(fs_worker.get_root_path(), root_path);
        fs::remove_all(root_path);
    }
};

TEST(FsSubWorkerGroup, CreateValid) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group/");
    FsSubWorkerGroup fs_worker1(root_path);
    EXPECT_EQ(root_path, fs_worker1.get_root_path());
    EXPECT_FALSE(static_cast<bool>(fs_worker1.err_code));

    FsSubWorkerGroup fs_worker2(fs_worker1);
    EXPECT_EQ(fs_worker2, fs_worker1);
    EXPECT_EQ(fs_worker2.err_code, std::errc(EEXIST));

    FsSubWorkerGroup fs_worker3(std::move(fs_worker1));
    EXPECT_EQ(fs_worker3, fs_worker2);
    EXPECT_EQ(fs_worker3.err_code, std::errc(EEXIST));

    fs::remove_all(root_path);
    FsSubWorkerGroup fs_worker4(std::move(root_path));
    EXPECT_EQ(fs_worker4, fs_worker2);
    EXPECT_FALSE(static_cast<bool>(fs_worker4.err_code));

    fs::remove_all(fs_worker4.get_root_path());
}

TEST_F(FixtureCreateFsSubWorkerGroupInvalid, ENOENT) {
    fs::path root_path = TEST_PATH / fs::path("input_output/not_existing_dir/fs_sub_worker_group/");
    FixtureCreateFsSubWorkerGroupInvalid::set_up(std::move(root_path), std::errc(ENOENT));
}

TEST_F(FixtureCreateFsSubWorkerGroupInvalid, ENAMETOOLONG) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path(std::string{_PC_NAME_MAX, 'x'});
    FixtureCreateFsSubWorkerGroupInvalid::set_up(std::move(root_path), std::errc(ENAMETOOLONG));
}

TEST_F(FixtureCreateFsSubWorkerGroupInvalid, ENOTDIR) {
    fs::path file_path = TEST_PATH "/input_output/file";
    std::ofstream file(file_path);
    file.close();
    ASSERT_TRUE(fs::exists(file_path));

    fs::path root_path = file_path / fs::path("fs_sub_worker_not_group/");
    FixtureCreateFsSubWorkerGroupInvalid::set_up(std::move(root_path), std::errc(ENOTDIR));

    fs::remove(file_path);
}

TEST(FsSubWorkerGroup, MoveRoot) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group");
    FsSubWorkerGroup fs_worker(create_worker_group(root_path));
    fs::path new_root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group_new");
    fs_worker.move_root(new_root_path);

    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
    EXPECT_EQ(fs_worker.get_root_path(), new_root_path);
    std::error_code ec;
    EXPECT_TRUE(fs::exists(new_root_path, ec));
    EXPECT_FALSE(fs::exists(root_path, ec));

    fs::remove_all(new_root_path);
}

TEST_F(FixtureFsSubWorkerGroupMoveRootInvalid, ENAMETOOLONG) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group/");
    fs::path new_root_path = TEST_PATH "/input_output" / fs::path(std::string{_PC_NAME_MAX, 'x'});
    FixtureFsSubWorkerGroupMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENAMETOOLONG));
}

TEST_F(FixtureFsSubWorkerGroupMoveRootInvalid, ENOENT) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group/");
    fs::path new_root_path = TEST_PATH / fs::path("input_output/not_existing_dir/fs_sub_worker_group");
    FixtureFsSubWorkerGroupMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENOENT));
}

TEST_F(FixtureFsSubWorkerGroupMoveRootInvalid, ENOTDIR) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group/");
    fs::path file_path = TEST_PATH "/input_output/file";
    std::ofstream file(file_path);
    file.close();
    ASSERT_TRUE(fs::exists(file_path));

    fs::path new_root_path = file_path / fs::path("fs_sub_worker_not_group/");
    FixtureFsSubWorkerGroupMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENOTDIR));
    fs::remove(file_path);
}

TEST(FsSubWorkerGroup, WorkWithGroupsFiles) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group/");
    FsSubWorkerGroup fs_worker = create_worker_group(root_path);
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    std::vector<int> groups{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::for_each(groups.begin(), groups.end(), [&fs_worker, &root_path](int group) {
        EXPECT_TRUE(fs_worker.add_group(group));
        std::error_code ec;
        EXPECT_TRUE(fs::exists(root_path / std::to_string(group), ec));
    });

    fs::path file_path = root_path / fs::path("test_move");
    std::ofstream file(file_path);
    ASSERT_TRUE(file.is_open());
    file << "This is test of FsSubWorkerAuthUsr::move_file_to_group_dir()";
    file.close();

    fs::path file_hlink_path = root_path / fs::path("test_move_hlink");
    for (int &group: groups) {
        std::error_code ec;
        fs::create_hard_link(file_path, file_hlink_path, ec);
        if (static_cast<bool>(ec)) {
            EXPECT_FALSE("creation of hard link failed. Part of the test skipped.\n");
            continue;
        }
        fs_worker.reset_error_code();
        fs::path file_name("moved_file");
        EXPECT_TRUE(fs_worker.move_file_to_group_dir(file_hlink_path, file_name, group));

        fs::path moved_file_path = root_path / std::to_string(group) / file_name;
        EXPECT_TRUE(fs::equivalent(file_path, moved_file_path, ec));
        EXPECT_TRUE(fs_worker.get_group_file(file_name, group).is_open());

        EXPECT_TRUE(fs_worker.remove_group_file(file_name, group));
        EXPECT_FALSE(fs::exists(moved_file_path, ec));
    }

    fs::remove_all(root_path);
}

TEST(FsSubWorkerGroup, WorkWithUsersFilesInvalidArgs) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group/");
    FsSubWorkerGroup fs_worker = create_worker_group(root_path);
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    int group = 0;
    EXPECT_TRUE(fs_worker.add_group(group));
    std::error_code ec;
    EXPECT_TRUE(fs::exists(root_path / std::to_string(group), ec));

    fs::path file_path = root_path / fs::path("test_move");
    std::ofstream file(file_path);
    ASSERT_TRUE(file.is_open());
    file << "This is test of FsSubWorkerAuthUsr::move_file_to_user_dir()";
    file.close();

    fs::path file_hlink_path = root_path / fs::path("test_move_hlink");
    fs::create_hard_link(file_path, file_hlink_path, ec);
    ASSERT_FALSE(static_cast<bool>(ec));

    fs_worker.reset_error_code();
    fs::path fname_moved("moved_file");
    EXPECT_TRUE(fs_worker.move_file_to_group_dir(file_hlink_path, fname_moved, group));

    fs::path moved_file_path = root_path / std::to_string(group) / fs::path("file_move");
    EXPECT_TRUE(fs::equivalent(file_path, moved_file_path, ec));
    EXPECT_TRUE(fs_worker.get_group_file(fname_moved, group).is_open());

    // group already exists
    EXPECT_FALSE(fs_worker.add_group(group));

    //EEXIST
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.move_file_to_group_dir(file_path, "file_move", group));
    EXPECT_EQ(fs_worker.err_code, std::errc(EEXIST));

    // ENAMETOOLONG
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.move_file_to_group_dir(file_path, std::string{_PC_NAME_MAX, 'x'}, group));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENAMETOOLONG));

    // ENOENT
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.move_file_to_group_dir(file_path, "/not_existing_dir/file_move", group));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENOENT));
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.remove_group_file("/not_existing_dir/file_move", group));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENOENT));

    EXPECT_TRUE(fs_worker.remove_group_file("file_move", group));
    EXPECT_FALSE(fs::exists(moved_file_path, ec));

    fs::remove_all(root_path);
}

TEST(FsSubWorkerGroup, WorkWithUsersDirs) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group/");
    FsSubWorkerGroup fs_worker = create_worker_group(root_path);
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    int group = 0;
    EXPECT_TRUE(fs_worker.add_group(group));
    std::error_code ec;
    EXPECT_TRUE(fs::exists(root_path / std::to_string(group), ec));

    fs::path group_subdir_path("group_subdir");
    EXPECT_TRUE(fs_worker.make_group_subdir(group_subdir_path, group));
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

    fs::path file_path = root_path / fs::path("test_move");
    std::ofstream file(file_path);
    ASSERT_TRUE(file.is_open());
    file << "This is test of FsSubWorkerAuthUsr::move_file_to_group_dir()";
    file.close();

    fs::path file_hlink_path = root_path / fs::path("test_move_hlink");
    fs::create_hard_link(file_path, file_hlink_path, ec);

    if (static_cast<bool>(ec)) {
        fs::remove_all(root_path);
        ASSERT_FALSE("creation of hard link failed. Part of the test skipped.\n");
    }

    fs::path fname_moved("moved_file");
    fs_worker.reset_error_code();
    EXPECT_TRUE(fs_worker.move_file_to_group_dir(file_hlink_path, group_subdir_path / fname_moved, group));
    EXPECT_TRUE(fs::equivalent(file_path, root_path / group_subdir_path / fname_moved, ec));
    EXPECT_TRUE(fs_worker.get_group_file(group_subdir_path / fname_moved, group).is_open());
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

    fs_worker.reset_error_code();
    EXPECT_TRUE(fs_worker.remove_dir(group_subdir_path, group));
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

    fs::remove_all(root_path);
}

TEST(FsSubWorkerGroup, WorkWithUsersDirsInvalidArgs) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_group/");
    FsSubWorkerGroup fs_worker = create_worker_group(root_path);
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    int group = 0;
    EXPECT_TRUE(fs_worker.add_group(group));
    std::error_code ec;
    EXPECT_TRUE(fs::exists(root_path / std::to_string(group), ec));

    fs::path group_subdir_path("user_subdir");
    EXPECT_TRUE(fs_worker.make_group_subdir(group_subdir_path, group));
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

    //EEXIST
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.make_group_subdir(group_subdir_path, group));
    EXPECT_EQ(fs_worker.err_code, std::errc(EEXIST));

    // ENAMETOOLONG
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.make_group_subdir(std::string{_PC_NAME_MAX, 'x'}, group));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENAMETOOLONG));

    // ENOENT
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.make_group_subdir("/not_existing_dir" / group_subdir_path, group));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENOENT));
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.remove_dir("not_existing_dir", group));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENOENT));

    fs::remove_all(root_path);
}
