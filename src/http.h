#ifndef TRC_HTTP_H
#define TRC_HTTP_H

#include <string>

bool http_post(const std::string &host, const std::string &uri, const std::string &raw_post_data);

#endif // TRC_HTTP_H

