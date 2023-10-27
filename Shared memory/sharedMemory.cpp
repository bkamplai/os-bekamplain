#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

const char* SHM_NAME = "my_shared_memory";
const size_t BUFFER_SIZE = 10 * sizeof(int);

int openSharedMemory(const char*);
void setMemorySize(int, size_t);
int* mapSharedMemory(int, size_t);
void writeToMemory(int*, size_t);
void readFromMemory(int*, size_t);

int main() {
    int shm_fd = openSharedMemory(SHM_NAME);
    setMemorySize(shm_fd, BUFFER_SIZE);
    int* buffer = mapSharedMemory(shm_fd, BUFFER_SIZE);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Failed to fork");
        return 1;
    } else if (pid == 0) {
        writeToMemory(buffer, BUFFER_SIZE);
    } else {
        wait(NULL);
        readFromMemory(buffer, BUFFER_SIZE);

        if (shm_unlink(SHM_NAME) == -1) {
            perror("Failed to unlink shared memory");
            return 1;
        }
    }

    if (munmap(buffer, BUFFER_SIZE) == -1) {
        perror("Failed to unmap shared memory");
        return 1;
    }

    return 0;
}

int openSharedMemory(const char* name) {
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        perror("Failed to open shared memory");
        exit(1);
    }

    return shm_fd;
}

void setMemorySize(int shm_fd, size_t size) {
    if (ftruncate(shm_fd, size) != 0) {
        perror("Faled to set the size of the shared memory object");
        exit(1);
    }
}

int* mapSharedMemory(int shm_fd, size_t size) {
    int* buffer = static_cast<int*>(mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    if (buffer == MAP_FAILED) {
        perror("Failed to map shared memory");
        exit(1);
    }

    return buffer;
}

void writeToMemory(int* buffer, size_t size) {
    cout << "Child writing to shared memory." << endl;

    for (long unsigned int i=0; i < size/sizeof(int); ++i) {
        buffer[i] = i * i;
    }
}

void readFromMemory(int* buffer, size_t size) {
    cout << "Parent reading from shared memory." << endl;

    for (long unsigned int i=0; i < size/sizeof(int); ++i) {
        cout << buffer[i] << " ";
    }

    cout << endl;
}