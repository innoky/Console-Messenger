#include <iostream>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>

using boost::asio::ip::tcp;

std::vector<std::shared_ptr<tcp::socket>> sockets;
std::mutex sockets_mutex;
 
void chat_session(std::shared_ptr<tcp::socket> socket) {
    try {
        for (;;) {
            char data[1024];
            boost::system::error_code error;
            size_t length = socket->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break;  // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error);  // Some other error.
                
            // Send received message to all connected clients
            std::lock_guard<std::mutex> lock(sockets_mutex); 
            for (auto& sock : sockets) {                     
                if (sock != socket) {   
                    boost::asio::write(*sock, boost::asio::buffer(data, length));
                }
            }    
        }        
    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
    {
        std::lock_guard<std::mutex> lock(sockets_mutex);
        
        sockets.erase(std::remove(sockets.begin(), sockets.end(), socket), sockets.end());
    }
}    
     
int main() {
    try {   
        boost::asio::io_context io_context;
        
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));
 
        for (;;) {
            std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(io_context);
            acceptor.accept(*socket);
            {
                std::lock_guard<std::mutex> lock(sockets_mutex);
                sockets.push_back(socket);
            }
            std::thread(chat_session, socket).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

