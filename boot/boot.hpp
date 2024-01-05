#pragma once

namespace art {
    struct boot_info;

    /**
     * @brief A bootloader-agnostic start function.
     * @param info A pointer to a boot_info.
     */
    void start(boot_info* info);

    /**
     * @brief Prepares an environment for kmain().
     * @param info A boot_info struct.
     */
    void ksetup(boot_info* info);

    /**
     * @brief The main()-equivalent function.
     * This function depends on ksetup() to perform basic initialization tasks.
     * @return Return code.
     */
    int kmain(int argc, char* argv[]);
}