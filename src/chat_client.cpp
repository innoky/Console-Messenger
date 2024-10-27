#include <iostream>
#include <boost/asio.hpp>
#include <string>
using boost::asio::ip::tcp;

int main() {
    try {
        
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        std::string ip_adress;
        std::cin >> ip_adress;
        boost::asio::connect(socket, resolver.resolve(ip_adress, "1234"));

        std::thread t([&socket]() {
            char data[1024];
            std::cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" << std::endl;
            std::cout << "┃  KZS ONLINE CHAT STARTED  ┃" << std::endl;
            std::cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" << std::endl << std::endl;

            std::cout << "Ваш сообщения идут без подписи / Сообщения собеседника подписаны анонимным юзером" << std::endl << std::endl;

            for (;;) {
                boost::system::error_code error;
                size_t length = socket.read_some(boost::asio::buffer(data), error);
                if (error == boost::asio::error::eof)
                    break;  // Connection closed cleanly by peer.
                else if (error)
                    throw boost::system::system_error(error);  // Some other error.
                std::cout << "ANONIMOUS USER > ";
                std::cout << data;
                std::cout << "\n";
            }
        });

        for (;;) {
            char line[1024];
            std::cin.getline(line, 1024);
            size_t length = strlen(line);
            boost::asio::write(socket, boost::asio::buffer(line, length));
        }

        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
