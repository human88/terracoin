#include "http.h"
#include "util.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/**
 * @brief simple http client.
 * @param host Host to submit http request to. ex: "www.myhost.com"
 * @param uri path where to submit request to. ex: "/some/random/uri/blah.ext"
 * @return boolean true on success, false otherwise.
 */
bool http_post(const std::string &host, const std::string &uri, const std::string &raw_post_data) {
    printf("Initiating http_post to host=%s at uri=%s\n", host.c_str(), uri.c_str());
    try {
        boost::asio::io_service io_service;

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, "http");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        // Try each endpoint until we successfully establish a connection.
        tcp::socket socket(io_service);
        boost::system::error_code error = boost::asio::error::host_not_found;
        while (error && endpoint_iterator != end) {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }
        if (error)
            throw boost::system::system_error(error);

        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "POST " << uri << " HTTP/1.0\r\n";
        request_stream << "Host: " << host << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Content-Length: " << raw_post_data.length() << "\r\n\r\n";
        request_stream << raw_post_data << "\r\n\r\n";
        //request_stream << "Connection: close\r\n\r\n";

        // Send the request.
        boost::asio::write(socket, request);

        // Read the response status line.
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        // Check that response is OK.
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            printf("Invalid response from %s\n", host.c_str());;
            return (false);
        }

        if (status_code != 200) {
            printf("Response returned with status code %d\n", status_code);
            return (false);
        } else {
            printf("200 OK from host/uri=%s%s\n", host.c_str(), uri.c_str());
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
            ; //std::cout << header << "\n";
        //std::cout << "\n";

        // Write whatever content we already have to output.
        //if (response.size() > 0)
        //    std::cout << &response;

        // Read until EOF, writing data to output as we go.
        while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
            ; //std::cout << &response;
        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        printf("Http exception: %s\n", e.what());
        return (false);
    }

    return (true);
}

