#pragma once
#include <Lib/Types/Uefi.hpp>
#include <Lib/Types/type_bool.hpp>
PUBLIC namespace QuantumNEC::Architecture {
    PUBLIC constexpr CONST auto NR_SYS_CALL { 20 };
    PUBLIC class Syscall
    {
        /*
         * 这个结构是这样的
         * 可以有超过三个系统调用
         * 前三个是属于消息处理的
         * 后面可以接着加上
         * 顺序就是枚举类SyscallFunction里的枚举类型顺序
         * 调用约定：RAX->返回值，RBX->功能号（SyscallFunction），RCX->（目标或者源），RDX -> 消息或者别的啥
         */
    public:
        enum class SyscallStatus : Lib::Types::uint32_t {
            ERROR,
            SUCCESS,
            NO_SYSCALL,
            DEADLOCK,
            NO_DESTINATION,
            NO_SOURCE,
        };
        enum class SyscallFunction : Lib::Types::uint32_t {
            MESSAGE_SEND,
            MESSAGE_RECEIVE,
            MESSAGE_SEND_RECEIVE,
        };
        using SyscallEntry = Lib::Types::FuncPtr< VOID, Lib::Types::Ptr< Architecture::CPUs::InterruptFrame > >;

    public:
        explicit Syscall( VOID ) noexcept;
        virtual ~Syscall( VOID ) noexcept = default;

    public:
        STATIC auto set_syscall_table( IN Lib::Types::uint64_t index, IN SyscallEntry entry ) -> VOID;
        STATIC auto get_syscall_table( VOID ) -> Lib::Types::Ptr< SyscallEntry > {
            return syscall_table;
        }

    public:
        /**
         * @brief 第一个系统调用：消息发送接收（第二个，第三个也是这个）
         */
        STATIC auto syscall_send_receive( IN Lib::Types::Ptr< Architecture::CPUs::InterruptFrame > frame ) -> VOID;

    private:
        inline STATIC SyscallEntry syscall_table[ NR_SYS_CALL ] { };
    };
}