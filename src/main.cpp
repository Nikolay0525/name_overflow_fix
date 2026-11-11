// name_overflow_fix: Kyubu port of coremaze's Cube-World-Name-Overflow-Fix.
//
// The /name command copies the new name into a 16-wchar buffer without checking its length. At 0x7E3E3 the game does
//     add esp,8 / cmp dword ptr [ebp-0x18],0 / jz ...
// where [ebp-0x18] is the length of the name. The stub rejects names of 16 or more characters with a chat message and
// jumps to the end of the handler (0x7E428); otherwise it runs the original compare and continues at 0x7E3EA.
// Like bad_item_id_fix, the stub chooses where execution continues, so it is kept and installed through patch_bytes.
//
// The chat message uses the game's own message functions (0xEB60 build message, 0x3AB30 print, 0x193E50 destroy) and
// the chat widget at GameController+0x800A14, exactly as the original did.
#include "../common.hpp"

#include <cwchar>

extern "C" {
unsigned int base = 0;
}

struct ColorRGBA { float red, green, blue, alpha; };
static ColorRGBA defaultColor = {1.0f, 1.0f, 1.0f, 1.0f};
extern "C" {
unsigned int defaultColorPtr = 0;
wchar_t defaultMessage[255];
unsigned int defaultMessagePtr = 0;
char msgObject[255];
unsigned int msgObjectPtr = 0;
}

static void ASMPrintMessage() {
    asm("push [_defaultMessagePtr]");
    asm("mov ecx, [_msgObjectPtr]");

    asm("mov eax, [_base]");
    asm("add eax, 0x0EB60");
    asm("call eax");        // builds the message object

    asm("mov ecx, [_base]");
    asm("add ecx, 0x36B1C8");
    asm("mov ecx, [ecx]");  // GameController
    asm("mov ecx, [ecx + 0x800A14]");   // chat widget

    asm("push [_defaultColorPtr]");
    asm("push [_msgObjectPtr]");
    asm("mov edx, [_base]");
    asm("add edx, 0x3AB30");
    asm("call edx");        // prints the message

    asm("mov ecx, [_msgObjectPtr]");
    asm("mov eax, [_base]");
    asm("add eax, 0x193E50");
    asm("call eax");        // destroys the message object
}

static void NameTooLong() {
    std::wcsncpy(defaultMessage, L"Character name too long\n", 254);
    defaultColor.red = 255 / 255.0f;
    defaultColor.green = 51 / 255.0f;
    defaultColor.blue = 51 / 255.0f;
    defaultColor.alpha = 1.0f;
    ASMPrintMessage();
}
extern "C" {
unsigned int NameTooLongPtr = 0;
}

void __declspec(naked) ASMHandleNameChange() {
    asm("add esp, 0x8");                    // original code
    asm("cmp dword ptr [ebp - 0x18], 0x10");
    asm("jb 0f");

    asm("call [_NameTooLongPtr]");          // the name is too long: say so and skip the copy
    asm("mov ecx, [_base]");
    asm("add ecx, 0x7E428");
    asm("jmp ecx");

    asm("0:");                              // the name fits
    asm("mov eax, [_base]");
    asm("add eax, 0x7E3EA");
    asm("cmp dword ptr [ebp - 0x18], 0");   // original comparison
    asm("jmp eax");
}

struct NameOverflowFix {
    explicit NameOverflowFix(kyubu::Mod& m) {
        const uintptr_t site = classic::at(m, 0x7E3E3);
        static const uint8_t want[] = {0x83, 0xC4, 0x08, 0x83, 0x7D, 0xE8, 0x00, 0x74, 0x3C};
        if (!classic::expect(m, site, want, sizeof want, "name_overflow_fix")) return;
        base = static_cast<unsigned>(classic::at(m, 0));
        defaultColorPtr = reinterpret_cast<unsigned>(&defaultColor);
        defaultMessagePtr = reinterpret_cast<unsigned>(&defaultMessage);
        msgObjectPtr = reinterpret_cast<unsigned>(&msgObject);
        NameTooLongPtr = reinterpret_cast<unsigned>(&NameTooLong);
        if (!classic::write_jmp(m, site, reinterpret_cast<const void*>(&ASMHandleNameChange), "name_overflow_fix")) return;
        m.info("name_overflow_fix: names of 16 or more characters are refused");
    }
};

KYUBU_MOD("name_overflow_fix", KY_SIDE_CLIENT, NameOverflowFix)
