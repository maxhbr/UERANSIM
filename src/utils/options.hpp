//
// This file is a part of UERANSIM open source project.
// Copyright (c) 2021 ALİ GÜNGÖR.
//
// The software and all associated files are licensed under GPL-3.0
// and subject to the terms and conditions defined in LICENSE file.
//

#pragma once

#include <cstring>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

static void NameControl(const std::string &name)
{
    if (name.empty())
        throw std::runtime_error("Invalid OptionItem: name is too short");
    for (char c : name)
    {
        if (c >= '0' && c <= '9')
            continue;
        if (c >= 'a' && c <= 'z')
            continue;
        if (c >= 'A' && c <= 'Z')
            continue;
        if (c == '-')
            continue;
        throw std::runtime_error("Invalid OptionItem: name contains illegal character");
    }
}

namespace opt
{

struct OptionItem
{
    std::optional<char> shortName{};
    std::optional<std::string> longName{};
    std::optional<std::string> description{};
    std::optional<std::string> argument{};

    OptionItem(const std::optional<char> &shortName, std::optional<std::string> longName,
               std::optional<std::string> description, std::optional<std::string> argument)
        : shortName(shortName), longName(std::move(longName)), description(std::move(description)),
          argument(std::move(argument))
    {
        // WARN: Using this->... because of std::move
        if (!this->shortName.has_value() && !this->longName.has_value())
            throw std::runtime_error("shortName and longName both cannot be empty");

        if (this->longName.has_value())
            NameControl(*this->longName);
        if (this->argument.has_value())
            NameControl(*this->argument);
    }
};

struct OptionsDescription
{
    std::string projectName{};
    std::string version{};
    std::string appDescription{};
    std::string copyright{};
    std::string programName{};
    std::vector<OptionItem> items{};
    std::vector<std::string> usages{};

    OptionsDescription(std::string projectName, std::string version, std::string appDescription, std::string copyright,
                       std::string programName, std::vector<std::string> usages)
        : projectName(std::move(projectName)), version(std::move(version)), appDescription(std::move(appDescription)),
          copyright(std::move(copyright)), programName(std::move(programName)), usages(std::move(usages))
    {
    }
};

class OptionsResult
{
  private:
    std::vector<std::string> m_positionalParams{};
    std::unordered_map<std::string, std::string> m_options{};
    OptionsDescription m_description;

  public:
    OptionsResult(int argc, char **argv, const OptionsDescription &desc);

  public:
    bool hasFlag(const OptionItem &item) const;
    int positionalCount() const;
    int count() const;

  public:
    [[noreturn]] void help() const;
    [[noreturn]] void version() const;
    [[noreturn]] void error(const std::string &msg) const;
};

} // namespace opt
