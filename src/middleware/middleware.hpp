#ifndef MIDDELWARE_HPP
#define MIDDELWARE_HPP

#include <set>

/**
 * @brief Middleware namespace
 *
 * @return true middle ware catch;
 */
namespace Middleware {
bool authentifications(int client, void *message);
bool registrations(int client, void *message);
bool doWelcome(int client, void *msg);

std::set<std::string> getExcludedAuthentications();
std::set<std::string> getExcludedRegistrations();
}  // namespace Middleware

#endif