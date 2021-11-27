// Copyright 2021 nat-s.skv@mail.ru

#include <gtest/gtest.h>
#include <working_with_filesystem.hpp>
#include <fstream>

//TODO:change-name
FsSubWorkerNotAuthUsr create(const fs::path &root_path) {
    FsSubWorkerNotAuthUsr fs_worker(root_path);
    EXPECT_EQ(root_path, fs_worker.get_root_path());
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
    auto sort_dirs = fs_worker.get_sort_dirs();
    std::for_each(sort_dirs.begin(), sort_dirs.end(),
                  [&root_path](const auto &dir) { EXPECT_TRUE(fs::exists(root_path / dir)); });
    return fs_worker;
}

class FixtureCreateFsSubWorkerNotAuthUsrInvalid : public ::testing::Test {
protected:
    static void set_up(fs::path &&root_path, std::errc &&exp_errc) {
        FsSubWorkerNotAuthUsr fs_worker1(root_path);
        EXPECT_EQ(fs::path(""), fs_worker1.get_root_path());
        EXPECT_EQ(fs_worker1.err_code, exp_errc);
        EXPECT_TRUE(fs_worker1.get_sort_dirs().empty());

        fs::remove_all(root_path);
        FsSubWorkerNotAuthUsr fs_worker2(std::move(root_path));
        EXPECT_EQ(fs_worker1, fs_worker2);
    }
};

class FixtureFsSubWorkerNotAuthUsrMoveRootInvalid : public ::testing::Test {
protected:
    static void set_up(fs::path &root_path, const fs::path &new_root_path, std::errc &&exp_errc) {
        FsSubWorkerNotAuthUsr fs_worker(root_path);
        EXPECT_EQ(root_path, fs_worker.get_root_path());
        EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
        auto sort_dirs = fs_worker.get_sort_dirs();
        std::for_each(sort_dirs.begin(), sort_dirs.end(),
                      [&root_path](const auto &dir) { EXPECT_TRUE(fs::exists(root_path / dir)); });

        fs_worker.move_root(new_root_path);
        EXPECT_EQ(fs_worker.err_code, exp_errc);
        EXPECT_EQ(fs_worker.get_root_path(), new_root_path);
        fs::remove_all(root_path);
    }
};

TEST(FsSubWorkerNotAuthUsr, CreateValid) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path("fs_sub_worker_not_auth_usr/");
    FsSubWorkerNotAuthUsr fs_worker1(root_path);
    EXPECT_EQ(root_path, fs_worker1.get_root_path());
    EXPECT_FALSE(static_cast<bool>(fs_worker1.err_code));
    auto sort_dirs1 = fs_worker1.get_sort_dirs();
    std::for_each(sort_dirs1.begin(), sort_dirs1.end(),
                  [&root_path](const auto &dir) { EXPECT_TRUE(fs::exists(root_path / dir)); });

    FsSubWorkerNotAuthUsr fs_worker2(fs_worker1);
    EXPECT_EQ(fs_worker2, fs_worker1);
    EXPECT_EQ(fs_worker2.err_code, std::errc(EEXIST));

    FsSubWorkerNotAuthUsr fs_worker3(std::move(fs_worker1));
    EXPECT_EQ(fs_worker3, fs_worker2);

    fs::remove_all(root_path);
    FsSubWorkerNotAuthUsr fs_worker4(std::move(root_path));
    EXPECT_EQ(fs_worker4, fs_worker2);
    EXPECT_EQ(sort_dirs1, fs_worker2.get_sort_dirs());

    fs::remove_all(fs_worker4.get_root_path());
}

TEST_F(FixtureCreateFsSubWorkerNotAuthUsrInvalid, ENOENT) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path("not_existing_dir/fs_sub_worker_not_auth_usr/");
    FixtureCreateFsSubWorkerNotAuthUsrInvalid::set_up(std::move(root_path), std::errc(ENOENT));
}

TEST_F(FixtureCreateFsSubWorkerNotAuthUsrInvalid, ENAMETOOLONG) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path(std::string{_PC_NAME_MAX, 'x'});
    FixtureCreateFsSubWorkerNotAuthUsrInvalid::set_up(std::move(root_path), std::errc(ENAMETOOLONG));
}

TEST_F(FixtureCreateFsSubWorkerNotAuthUsrInvalid, ENOTDIR) {
    fs::path file_path = TEST_PATH "/input_output/file";
    std::ofstream file(file_path);
    file.close();
    ASSERT_TRUE(fs::exists(file_path));

    fs::path root_path = file_path / fs::path("fs_sub_worker_not_auth_usr/");
    FixtureCreateFsSubWorkerNotAuthUsrInvalid::set_up(std::move(root_path), std::errc(ENOTDIR));

    fs::remove(file_path);
}

TEST(FsSubWorkerNotAuthUsr, MoveRoot) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path("fs_sub_worker_not_auth_usr/");
    FsSubWorkerNotAuthUsr fs_worker(create(root_path));
    fs::path new_root_path = TEST_PATH "/input_output" / fs::path("fs_sub_worker_not_auth_usr_new/");
    fs_worker.move_root(new_root_path);
    EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
    EXPECT_EQ(fs_worker.get_root_path(), new_root_path);
    fs::remove_all(new_root_path);
}

TEST_F(FixtureFsSubWorkerNotAuthUsrMoveRootInvalid, ENAMETOOLONG) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path("fs_sub_worker_not_auth_usr/");
    fs::path new_root_path = TEST_PATH "/input_output" / fs::path(std::string{_PC_NAME_MAX, 'x'});
    FixtureFsSubWorkerNotAuthUsrMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENAMETOOLONG));
}

TEST_F(FixtureFsSubWorkerNotAuthUsrMoveRootInvalid, ENOENT) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path("fs_sub_worker_not_auth_usr/");
    fs::path new_root_path = TEST_PATH "/input_output" / fs::path("not_existing_dir/fs_sub_worker_not_auth_usr/");
    FixtureFsSubWorkerNotAuthUsrMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENOENT));
}

TEST_F(FixtureFsSubWorkerNotAuthUsrMoveRootInvalid, ENOTDIR) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path("fs_sub_worker_not_auth_usr/");
    fs::path file_path = TEST_PATH "/input_output/file";
    std::ofstream file(file_path);
    file.close();
    ASSERT_TRUE(fs::exists(file_path));

    fs::path new_root_path = file_path / fs::path("fs_sub_worker_not_auth_usr/");
    FixtureFsSubWorkerNotAuthUsrMoveRootInvalid::set_up(root_path, new_root_path, std::errc(ENOTDIR));
    fs::remove(file_path);
}

TEST(FsSubWorkerNotAuthUsr, CreateDayDirAndRemoveExpired) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path("fs_sub_worker_not_auth_usr/");
    FsSubWorkerNotAuthUsr fs_worker(create(root_path));
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));
    std::vector<fs::path> dates = {"10-10-2020", "11-10-2020", "12-10-2020"};

    std::for_each(dates.begin(), dates.end(),
                  [&fs_worker](const auto &date) {
                      EXPECT_TRUE(fs_worker.create_day_dir(static_cast<std::string>(date)));
                      EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
                      EXPECT_TRUE(fs::exists(fs_worker.get_root_path() / date));
                  });

    auto sort_dirs = fs_worker.get_sort_dirs();
    EXPECT_EQ(dates, sort_dirs);

    for (size_t i = sort_dirs.size() + 1ul; i > 0ul; --i) {
        fs_worker.reset_error_code();
        fs_worker.set_expiration_date(i);
        EXPECT_TRUE(fs_worker.remove_expired_dirs());

        std::for_each(dates.begin(), dates.begin() + static_cast<int>(i),
                      [&root_path](const auto &dir) { EXPECT_TRUE(fs::exists(root_path / dir)); });
        std::for_each(dates.begin() + static_cast<int>(i), dates.end(),
                      [&root_path](const auto &dir) { EXPECT_FALSE(fs::exists(root_path / dir)); });
    }

    fs::remove_all(root_path);
}

TEST(FsSubWorkerNotAuthUsr, MoveAndGetFile) {
    fs::path root_path = TEST_PATH "/input_output" / fs::path("fs_sub_worker_not_auth_usr/");
    FsSubWorkerNotAuthUsr fs_worker(create(root_path));
    ASSERT_FALSE(static_cast<bool>(fs_worker.err_code));

    fs::path file_path = root_path / fs::path("test_move");
    std::ofstream file(file_path);
    ASSERT_TRUE(file.is_open());
    file << "This is test of FsSubWorkerNotAuthUsr::move_file()";
    file.close();

    std::vector<fs::path> dates = {"10-10-2020", "11-10-2020", "12-10-2020"};

    std::for_each(dates.begin(), dates.end(),
                  [&fs_worker](const auto &date) {
                      EXPECT_TRUE(fs_worker.create_day_dir(static_cast<std::string>(date)));
                      EXPECT_FALSE(static_cast<bool>(fs_worker.err_code));
                      EXPECT_TRUE(fs::exists(fs_worker.get_root_path() / date));
                  });

    auto sort_dirs = fs_worker.get_sort_dirs();
    EXPECT_EQ(dates, sort_dirs);

    fs::path file_hlink_path = root_path / fs::path("test_move_hlink");
    for (const auto &date_dir: sort_dirs) {
        std::error_code err_code;
        fs::create_hard_link(file_path, file_hlink_path, err_code);
        if (static_cast<bool>(err_code)) {
            EXPECT_TRUE(false&&"creation of hard link failed. Part of the test skipped.\n");
            continue;
        }
        fs_worker.reset_error_code();
        EXPECT_TRUE(fs_worker.move_file_to_fs(file_path, "file_move", date_dir));

        fs::path moved_file_path =  fs::path(root_path / date_dir / file_path);
        EXPECT_TRUE(fs::equivalent(file_hlink_path, moved_file_path));
    }

    fs::remove_all(root_path);
}





// сделать несколько отдельных тестов на каждую функцию, и еше пару тестов на весь функционал