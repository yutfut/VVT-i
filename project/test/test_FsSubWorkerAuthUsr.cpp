// Copyright 2021 nat-s.skv@mail.ru

#include <gtest/gtest.h>
#include <working_with_fs_lib.hpp>
#include <fstream>

FsSubWorkerAuthUsr create_worker_group(const fs::path &root_path) {
    FsSubWorkerAuthUsr fs_worker(root_path);
    EXPECT_EQ(root_path, fs_worker.get_root_path());
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
    return fs_worker;
}

class FixtureCreateFsSubWorkerGroupInvalid : public ::testing::Test {
protected:
    static void set_up(fs::path &&root_path, std::errc &&exp_errc) {
        FsSubWorkerAuthUsr fs_worker1(root_path);
        EXPECT_EQ(fs::path(""), fs_worker1.get_root_path());
        EXPECT_EQ(fs_worker1.err_code, exp_errc);

        FsSubWorkerAuthUsr fs_worker2(std::move(root_path));
        EXPECT_EQ(fs_worker1, fs_worker2);
        EXPECT_EQ(fs_worker2.err_code, exp_errc);
    }
};

class FixtureFsSubWorkerGroupMoveRootInvalid : public ::testing::Test {
protected:
    static void set_up(fs::path &root_path, const fs::path &new_root_path, std::errc &&exp_errc) {
        FsSubWorkerAuthUsr fs_worker(root_path);
        EXPECT_EQ(root_path, fs_worker.get_root_path());
        EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

        fs_worker.move_root(new_root_path);
        EXPECT_EQ(fs_worker.err_code, exp_errc);
        EXPECT_EQ(fs_worker.get_root_path(), root_path);
        fs::remove_all(root_path);
    }
};

TEST(FsSubWorkerAuthUsr, CreateValid) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    FsSubWorkerAuthUsr fs_worker1(root_path);
    EXPECT_EQ(root_path, fs_worker1.get_root_path());
    EXPECT_FALSE(static_cast<bool>(fs_worker1.err_code));

    FsSubWorkerAuthUsr fs_worker2(fs_worker1);
    EXPECT_EQ(fs_worker2, fs_worker1);
    EXPECT_EQ(fs_worker2.err_code, std::errc(EEXIST));

    FsSubWorkerAuthUsr fs_worker3(std::move(fs_worker1));
    EXPECT_EQ(fs_worker3, fs_worker2);
    EXPECT_EQ(fs_worker3.err_code, std::errc(EEXIST));

    fs::remove_all(root_path);
    FsSubWorkerAuthUsr fs_worker4(std::move(root_path));
    EXPECT_EQ(fs_worker4, fs_worker2);
    EXPECT_FALSE(static_cast<bool>(fs_worker4.err_code));

    fs::remove_all(fs_worker4.get_root_path());
}

TEST_F(FixtureCreateFsSubWorkerGroupInvalid, ENOENT) {
    fs::path root_path = TEST_PATH / fs::path("input_output/not_existing_dir/fs_sub_worker_auth_usr/");
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

    fs::path root_path = file_path / fs::path("fs_sub_worker_not_auth_usr/");
    FixtureCreateFsSubWorkerGroupInvalid::set_up(std::move(root_path), std::errc(ENOTDIR));

    fs::remove(file_path);
}

TEST(FsSubWorkerAuthUsr, MoveRoot) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    FsSubWorkerAuthUsr fs_worker(create_worker_group(root_path));
    fs::path new_root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr_new/");
    fs_worker.move_root(new_root_path);

    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
    EXPECT_EQ(fs_worker.get_root_path(), new_root_path);
    std::error_code ec;
    EXPECT_TRUE(fs::exists(new_root_path, ec));
    EXPECT_FALSE(fs::exists(root_path, ec));

    fs::remove_all(new_root_path);
}

TEST_F(FixtureFsSubWorkerGroupMoveRootInvalid, ENAMETOOLONG) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    fs::path new_root_path = TEST_PATH "/input_output" / fs::path(std::string{_PC_NAME_MAX, 'x'});
    FixtureFsSubWorkerGroupMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENAMETOOLONG));
}

TEST_F(FixtureFsSubWorkerGroupMoveRootInvalid, ENOENT) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    fs::path new_root_path = TEST_PATH / fs::path("input_output/not_existing_dir/fs_sub_worker_auth_usr/");
    FixtureFsSubWorkerGroupMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENOENT));
}

TEST_F(FixtureFsSubWorkerGroupMoveRootInvalid, ENOTDIR) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    fs::path file_path = TEST_PATH "/input_output/file";
    std::ofstream file(file_path);
    file.close();
    ASSERT_TRUE(fs::exists(file_path));

    fs::path new_root_path = file_path / fs::path("fs_sub_worker_not_auth_usr/");
    FixtureFsSubWorkerGroupMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENOTDIR));
    fs::remove(file_path);
}

TEST(FsSubWorkerAuthUsr, WorkWithUsersFiles) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    FsSubWorkerAuthUsr fs_worker = create_worker_group(root_path);
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    std::vector<int> users{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::for_each(users.begin(), users.end(), [&fs_worker, &root_path](int user) {
        EXPECT_TRUE(fs_worker.add_user(user));
        std::error_code ec;
        EXPECT_TRUE(fs::exists(root_path / std::to_string(user), ec));
    });

    fs::path file_path = root_path / fs::path("test_move");
    std::ofstream file(file_path);
    ASSERT_TRUE(file.is_open());
    file << "This is test of FsSubWorkerAuthUsr::move_file_to_user_dir()";
    file.close();

    fs::path file_hlink_path = root_path / fs::path("test_move_hlink");
    for (int &user: users) {
        std::error_code ec;
        fs::create_hard_link(file_path, file_hlink_path, ec);
        if (static_cast<bool>(ec)) {
            EXPECT_FALSE("creation of hard link failed. Part of the test skipped.\n");
            continue;
        }
        fs_worker.reset_error_code();
        fs::path file_name("moved_file");
        EXPECT_TRUE(fs_worker.move_file_to_user_dir(file_hlink_path, file_name, user));

        fs::path moved_file_path = root_path / std::to_string(user) / file_name;
        EXPECT_TRUE(fs::equivalent(file_path, moved_file_path, ec));
        EXPECT_TRUE(fs_worker.get_user_file(file_name, user).is_open());

        EXPECT_TRUE(fs_worker.remove_user_file(file_name, user));
        EXPECT_FALSE(fs::exists(moved_file_path, ec));
    }

    fs::remove_all(root_path);
}

TEST(FsSubWorkerAuthUsr, WorkWithUsersFilesInvalidArgs) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    FsSubWorkerAuthUsr fs_worker = create_worker_group(root_path);
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    int user = 0;
    EXPECT_TRUE(fs_worker.add_user(user));
    std::error_code ec;
    EXPECT_TRUE(fs::exists(root_path / std::to_string(user), ec));

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
    EXPECT_TRUE(fs_worker.move_file_to_user_dir(file_hlink_path, fname_moved, user));

    fs::path moved_file_path = root_path / std::to_string(user) / fs::path("file_move");
    EXPECT_TRUE(fs::equivalent(file_path, moved_file_path, ec));
    EXPECT_TRUE(fs_worker.get_user_file(fname_moved, user).is_open());

    // user already exists
    EXPECT_FALSE(fs_worker.add_user(user));

    //EEXIST
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.move_file_to_user_dir(file_path, "file_move", user));
    EXPECT_EQ(fs_worker.err_code, std::errc(EEXIST));

    // ENAMETOOLONG
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.move_file_to_user_dir(file_path, std::string{_PC_NAME_MAX, 'x'}, user));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENAMETOOLONG));

    // ENOENT
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.move_file_to_user_dir(file_path, "/not_existing_dir/file_move", user));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENOENT));
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.remove_user_file("/not_existing_dir/file_move", user));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENOENT));

    EXPECT_TRUE(fs_worker.remove_user_file("file_move", user));
    EXPECT_FALSE(fs::exists(moved_file_path, ec));

    fs::remove_all(root_path);
}

TEST(FsSubWorkerAuthUsr, WorkWithUsersDirs) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    FsSubWorkerAuthUsr fs_worker = create_worker_group(root_path);
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    int user = 0;
    EXPECT_TRUE(fs_worker.add_user(user));
    std::error_code ec;
    EXPECT_TRUE(fs::exists(root_path / std::to_string(user), ec));

    fs::path user_subdir_path("user_subdir");
    EXPECT_TRUE(fs_worker.make_user_subdir(user_subdir_path, user));
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

    fs::path file_path = root_path / fs::path("test_move");
    std::ofstream file(file_path);
    ASSERT_TRUE(file.is_open());
    file << "This is test of FsSubWorkerAuthUsr::move_file_to_user_dir()";
    file.close();

    fs::path file_hlink_path = root_path / fs::path("test_move_hlink");
    fs::create_hard_link(file_path, file_hlink_path, ec);

    if (static_cast<bool>(ec)) {
        fs::remove_all(root_path);
        ASSERT_FALSE("creation of hard link failed. Part of the test skipped.\n");
    }

    fs::path fname_moved("moved_file");
    fs_worker.reset_error_code();
    EXPECT_TRUE(fs_worker.move_file_to_user_dir(file_hlink_path, user_subdir_path / fname_moved, user));
    EXPECT_TRUE(fs::equivalent(file_path, root_path / user_subdir_path / fname_moved, ec));
    EXPECT_TRUE(fs_worker.get_user_file(user_subdir_path / fname_moved, user).is_open());
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

    fs_worker.reset_error_code();
    EXPECT_TRUE(fs_worker.remove_dir(user_subdir_path, user));
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

    fs::remove_all(root_path);
}

TEST(FsSubWorkerAuthUsr, WorkWithUsersDirsInvalidArgs) {
    fs::path root_path = TEST_PATH / fs::path("input_output/fs_sub_worker_auth_usr/");
    FsSubWorkerAuthUsr fs_worker = create_worker_group(root_path);
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    int user = 0;
    EXPECT_TRUE(fs_worker.add_user(user));
    std::error_code ec;
    EXPECT_TRUE(fs::exists(root_path / std::to_string(user), ec));

    fs::path user_subdir_path("user_subdir");
    EXPECT_TRUE(fs_worker.make_user_subdir(user_subdir_path, user));
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));

    //EEXIST
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.make_user_subdir(user_subdir_path, user));
    EXPECT_EQ(fs_worker.err_code, std::errc(EEXIST));

    // ENAMETOOLONG
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.make_user_subdir(std::string{_PC_NAME_MAX, 'x'}, user));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENAMETOOLONG));

    // ENOENT
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.make_user_subdir("/not_existing_dir"/user_subdir_path, user));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENOENT));
    fs_worker.reset_error_code();
    EXPECT_FALSE(fs_worker.remove_dir("not_existing_dir", user));
    EXPECT_EQ(fs_worker.err_code, std::errc(ENOENT));

    fs::remove_all(root_path);
}
