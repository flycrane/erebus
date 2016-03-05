#pragma once

#include "byte_order.h"

struct IDisassembler {
        public:
                virtual ~IDisassembler();

                virtual void setByteOrder(ByteOrder byteOrder);
};
