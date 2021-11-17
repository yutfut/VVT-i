#include "main_server_settings.h"

const std::vector<std::string> MainServerSettings::valid_properties = {
    "http", "count_workflows", "access_log", "error_log", "server"
};

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
    switch (number_of_property) {
    case COUNT_WORKFLOWS_NUMBER: {
        try {
            this->count_workflows = stoi(value.substr(begin, value_length));
        }
        catch (std::exception& e) {
            throw "Count workflows can be only integer";
        }
        if (this->count_workflows <= 0) {
            throw "Count workflows can be only greater than zero";
        }
        break;
    }
    case ACCESS_LOG_NUMBER:
        this->access_log_filename = value.substr(begin, value_length);
        break;
    case ERROR_LOG_NUMBER:
        this->error_log_filename = value.substr(begin, value_length);
        break;
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

int MainServerSettings::get_lexeme(const std::string& config, int& pos, const std::vector<std::string>& valid_properties)
{
    while (isspace(config[pos]) && config[pos] != '\n')
    {
        ++pos;
    }

    if (config[pos] == '\n')
    {
        ++pos;
        return L_NEW_LINE;
    }

    if (config[pos] == '{')
    {
        ++pos;
        return L_BRACE_OPEN;
    }

    if (config[pos] == '}')
    {
        ++pos;
        return L_BRACE_CLOSE;
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
            return L_VALUE;
        }
    }

    if (config.substr(pos, sizeof("http") - 1) == "http")
    {
        pos += sizeof("http") - 1;
        return L_PROTOCOL;
    }

    for (auto iter = valid_properties.begin(); iter != valid_properties.end(); ++iter)
    {
        if (*iter == config.substr(pos, iter->size()))
        {
            pos += iter->size() + 1; // +1 to skip a colon or space
            if (*iter == "server")
            {
                return L_SERVER_START;
            }
            return L_KEY;
        }
    }

    return L_ERR;
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
    file.close();

    return str;
}

void MainServerSettings::parse_config()
{
    std::string config_text;

    config_text = get_string_from_file(this->config_filename);
    int pos = 0;

    state_t stages[S_COUNT][L_COUNT] = {
        /*L_PROTOCOL     L_BRACE_OPEN L_BRACE_CLOSE L_NEW_LINE L_KEY L_VALUE L_SERVER_START L_SERVER_END*/
        /*S_START*/ {S_BRACE_OPEN, S_ERR, S_ERR, S_ERR, S_ERR, S_ERR, S_ERR, S_ERR},
        /*S_BRACE_OPEN*/ {S_ERR, S_KEY, S_ERR, S_ERR, S_ERR, S_ERR, S_ERR, S_ERR},
        /*S_KEY*/ {S_ERR, S_ERR, S_END, S_KEY, S_VALUE, S_ERR, S_SERVER_START, S_KEY},
        /*S_VALUE*/ {S_ERR, S_ERR, S_ERR, S_ERR, S_ERR, S_KEY, S_ERR, S_ERR},
        /*S_SERVER_START*/ {S_ERR, S_KEY, S_ERR, S_ERR, S_ERR, S_ERR, S_ERR, S_ERR},
    };

    state_t state = S_START;
    lexeme_t lexeme;
    int pos_before;
    int property_number;

    bool is_server_adding = false, has_server_added = false;

    while (state != S_END && state != S_ERR)
    {
        pos_before = pos;
        if (is_server_adding)
        {
            lexeme = static_cast<lexeme_t>(get_lexeme(config_text, pos, this->server.valid_properties));
        }
        else
        {
            lexeme = static_cast<lexeme_t>(get_lexeme(config_text, pos, this->valid_properties));
        }
        if (lexeme == L_ERR)
        {
            state = S_ERR;
            continue;
        }

        if (stages[state][lexeme] == S_ERR)
        {
            state = S_ERR;
            continue;
        }

        if (lexeme == L_KEY)
        {
            if (is_server_adding)
            {
                property_number = this->server.get_number_of_property(
                    config_text.substr(pos_before, pos - pos_before));
                if (property_number == -1)
                {
                    state = S_ERR;
                    continue;
                }
            }
            else
            {
                property_number = this->get_number_of_property(
                    config_text.substr(pos_before, pos - pos_before));
                if (property_number == -1)
                {
                    state = S_ERR;
                    continue;
                }
            }
        }
        else if (lexeme == L_VALUE)
        {
            if (is_server_adding)
            {
                try
                {
                    this->server.set_property(property_number,
                        config_text.substr(pos_before, pos - pos_before));
                }
                catch (std::exception& e)
                {
                    state = S_ERR;
                    continue;
                }
            }
            else
            {
                try
                {
                    this->set_property(property_number, config_text.substr(pos_before, pos - pos_before));
                }
                catch (std::exception& e)
                {
                    state = S_ERR;
                    continue;
                }
            }
        }
        else if (lexeme == L_SERVER_START && stages[state][lexeme] == S_SERVER_START)
        {
            if (is_server_adding || has_server_added)
            {
                state = S_ERR;
                continue;
            }
            is_server_adding = true;
            has_server_added = true;
        }
        else if (lexeme == L_BRACE_CLOSE && is_server_adding)
        {
            is_server_adding = false;
            lexeme = L_SERVER_END;
        }

        state = stages[state][lexeme];
    }
    if (state == S_ERR)
    {
        throw "Invalid config file, pos = " + std::to_string(pos);
    }
}
