#include "main_server_settings.h"

const int HTTP_LENGTH = 4;

MainServerSettings::MainServerSettings(const std::string& config_filename) : config_filename(std::move(config_filename)) {
    this->parse_config();
}

int MainServerSettings::get_number_of_property(const std::string& property) {
    int begin = 0;
    while (isspace(property[begin])) {
        ++begin;
    }
    int property_length = (property[property.length() - 1] == ':') ? property.length() - begin - 1 : property.length() - begin;

    for (auto iter = this->valid_properties.begin(); iter != this->valid_properties.end(); ++iter) {
        if (property.substr(begin, property_length) == *iter) {
            return iter - this->valid_properties.begin();
        }
    }

    return -1;
}

void MainServerSettings::set_property(int number_of_property, const std::string& value) {
    int begin = 0;
    while (isspace(value[begin])) {
        ++begin;
    }
    int value_length = (value[value.length() - 1] == ';') ? value.length() - begin - 1 : value.length() - begin;

    numbers_of_properties_t number = (numbers_of_properties_t)number_of_property;
    switch (number) {
    case numbers_of_properties_t::COUNT_WORKFLOWS_NUMBER:
        this->set_count_workflows(value, begin, value_length);
        break;
    case numbers_of_properties_t::ACCESS_LOG_NUMBER:
        this->access_log_filename = value.substr(begin, value_length);
        break;
    case numbers_of_properties_t::ERROR_LOG_NUMBER:
        this->error_log_filename = value.substr(begin, value_length);
        break;
    }
}

void MainServerSettings::set_count_workflows(const std::string& value, int begin, int value_length) {
    try {
        this->count_workflows = stoi(value.substr(begin, value_length));
    }
    catch (std::exception& e) {
        throw "Count workflows can be only integer";
    }
    if (this->count_workflows <= 0) {
        throw "Count workflows can be only greater than zero";
    }
}

int MainServerSettings::get_count_workflows() {
    return this->count_workflows;
}

ServerSettings MainServerSettings::get_server() {
    return this->server;
}

std::string MainServerSettings::get_access_log_filename() {
    return this->access_log_filename;
}

std::string MainServerSettings::get_error_log_filename() {
    return this->error_log_filename;
}

MainServerSettings::lexeme_t MainServerSettings::get_lexeme_t(const std::string& config, int& pos, const std::array<std::string, 5>& valid_properties)
{
    while (isspace(config[pos]) && config[pos] != '\n')
    {
        ++pos;
    }

    if (config[pos] == '\n')
    {
        ++pos;
        return lexeme_t::L_NEW_LINE;
    }

    if (config[pos] == '{')
    {
        ++pos;
        return lexeme_t::L_BRACE_OPEN;
    }

    if (config[pos] == '}')
    {
        ++pos;
        return lexeme_t::L_BRACE_CLOSE;
    }

    for (int i = pos; i < config.size() && config[i] != '\n'; ++i)
    {
        if (config[i] == ':')
        {
            break;
        }
        if (config[i] == ';')
        {
            pos = i + 1; // +1 to skip the semicolon
            return lexeme_t::L_VALUE;
        }
    }

    if (config.substr(pos, HTTP_LENGTH) == "http")
    {
        pos += HTTP_LENGTH;
        return lexeme_t::L_PROTOCOL;
    }

    for (auto iter = valid_properties.begin(); iter != valid_properties.end(); ++iter)
    {
        if (*iter == config.substr(pos, iter->size()))
        {
            pos += iter->size() + 1; // +1 to skip a colon or space
            if (*iter == "server")
            {
                return lexeme_t::L_SERVER_START;
            }
            if (*iter == "database")
            {
                return lexeme_t::L_DATABASE_START;
            }
            return lexeme_t::L_KEY;
        }
    }

    return lexeme_t::L_ERR;
}

std::string MainServerSettings::get_string_from_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw "No such file " + filename;
    }

    size_t file_size = 0;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string str;
    str.resize(file_size);
    for (char& i : str)
    {
        file.get(i);
    }

    return str;
}

void MainServerSettings::parse_config()
{
    std::string config_text;

    config_text = this->get_string_from_file(this->config_filename);
    int pos = 0;

    // Finite state_t machine
    // Input signals are presented in lexeme_t
    // state_ts are presented in state_t
    state_t stages[6][10] = {
        /* L_PROTOCOL  L_BRACE_OPEN  L_BRACE_CLOSE  L_NEW_LINE  L_KEY  L_VALUE  L_SERVER_START  L_DATABASE_START  L_DATABASE_END  L_SERVER_END*/
        /*S_START*/  {state_t::S_BRACE_OPEN, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR},
        /*S_BRACE_OPEN*/  {state_t::S_ERR, state_t::S_KEY, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_BRACE_OPEN, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR},
        /*S_KEY*/  {state_t::S_ERR, state_t::S_ERR, state_t::S_END, state_t::S_KEY, state_t::S_VALUE, state_t::S_ERR, state_t::S_SERVER_START, state_t::S_DATABASE_START, state_t::S_KEY, state_t::S_KEY},
        /*S_VALUE*/  {state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_KEY, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR},
        /*S_SERVER_START*/  {state_t::S_ERR, state_t::S_KEY, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR},
        /*S_DATABASE_START*/  {state_t::S_ERR, state_t::S_KEY, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR, state_t::S_ERR},
    };

    state_t state_t = state_t::S_START;
    lexeme_t lexeme_t;
    int pos_before;
    int property_number;

    bool is_server_adding = false, has_server_added = false;
    bool is_database_adding = false, has_database_added = false;

    while (state_t != state_t::S_END && state_t != state_t::S_ERR)
    {
        pos_before = pos;

        if (is_server_adding)
        {
            if (is_database_adding)
            {
                lexeme_t = get_lexeme_t(config_text, pos, this->server.database_valid_properties);
            }
            else
            {
                lexeme_t = get_lexeme_t(config_text, pos, this->server.valid_properties);
            }
        }
        else
        {
            lexeme_t = get_lexeme_t(config_text, pos, this->valid_properties);
        }

        int _i = (int)state_t, _j = (int)lexeme_t;
        if (lexeme_t == lexeme_t::L_ERR || stages[_i][_j] == state_t::S_ERR)
        {
            state_t = state_t::S_ERR;
            continue;
        }

        if (lexeme_t == lexeme_t::L_KEY)
        {
            if (!is_server_adding)
            {
                property_number = this->get_number_of_property(
                    config_text.substr(pos_before, pos - pos_before));
                if (property_number == -1)
                {
                    state_t = state_t::S_ERR;
                    continue;
                }


                int i = (int)state_t, j = (int)lexeme_t;
                state_t = stages[i][j];
                continue;
            }

            if (is_database_adding)
            {
                property_number = this->server.get_number_of_database_property(
                    config_text.substr(pos_before, pos - pos_before));
            }
            else
            {
                property_number = this->server.get_number_of_property(
                    config_text.substr(pos_before, pos - pos_before));
            }
            if (property_number == -1)
            {
                state_t = state_t::S_ERR;
                continue;
            }
        }
        else if (lexeme_t == lexeme_t::L_VALUE)
        {
            if (!is_server_adding)
            {
                try
                {
                    this->set_property(property_number, config_text.substr(pos_before, pos - pos_before));
                }
                catch (std::exception& e)
                {
                    state_t = state_t::S_ERR;
                    continue;
                }

                int i = (int)state_t, j = (int)lexeme_t;
                state_t = stages[i][j];
                continue;
            }

            try
            {
                if (is_database_adding)
                {
                    this->server.set_database_property(property_number,
                        config_text.substr(pos_before, pos - pos_before));

                }
                else
                {
                    this->server.set_property(property_number,
                        config_text.substr(pos_before, pos - pos_before));
                }
            }
            catch (std::exception& e)
            {
                state_t = state_t::S_ERR;
                continue;
            }
        }
        else if (lexeme_t == lexeme_t::L_SERVER_START && stages[_i][_j] == state_t::S_SERVER_START)
        {
            if (is_server_adding || has_server_added)
            {
                state_t = state_t::S_ERR;
                continue;
            }
            is_server_adding = true;
            has_server_added = true;
        }
        else if (lexeme_t == lexeme_t::L_DATABASE_START && stages[_i][_j] == state_t::S_DATABASE_START)
        {
            if (!is_server_adding || is_database_adding || has_database_added)
            {
                state_t = state_t::S_ERR;
                continue;
            }
            is_database_adding = true;
            has_database_added = true;
        }
        else if (lexeme_t == lexeme_t::L_BRACE_CLOSE && is_database_adding)
        {
            is_database_adding = false;
            lexeme_t = lexeme_t::L_DATABASE_END;
        }
        else if (lexeme_t == lexeme_t::L_BRACE_CLOSE && is_server_adding)
        {
            is_server_adding = false;
            lexeme_t = lexeme_t::L_SERVER_END;
        }

        int i = (int)state_t, j = (int)lexeme_t;
        state_t = stages[i][j];
    }

    if (state_t == state_t::S_ERR)
    {
        throw "Invalid config file, pos = " + std::to_string(pos);
    }
}
