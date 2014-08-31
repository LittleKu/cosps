using System;

namespace CommonConsoleApp
{
    public class ByteBuffer
    {
        private byte[] m_buffer;
        private int m_count;

        public ByteBuffer() : this(256)
        {
        }

        public ByteBuffer(int capacity)
        {
            if (capacity < 16)
            {
                capacity = 16;
            }
            m_buffer = null;
            m_count = 0;

            EnsureCapacity(capacity);
        }

        private void EnsureCapacity(int minCap)
        {
            int cap = ((m_buffer == null) ? 0 : m_buffer.Length);
            if (cap >= minCap)
            {
                return;
            }

            cap = 1;
            while (cap < minCap)
            {
                cap <<= 1;
            }

            byte[] buffer = new byte[cap];
            if (m_buffer != null && m_count > 0)
            {
                Buffer.BlockCopy(m_buffer, 0, buffer, 0, m_count);
            }
            m_buffer = buffer;
        }

        public void AddBytes(byte[] buffer, int offset, int count)
        {
            if ((buffer == null) || (buffer.Length <= 0) || (offset < 0) || ((offset + count) > buffer.Length))
            {
                return;
            }
            EnsureCapacity(m_count + count);
            Buffer.BlockCopy(buffer, offset, m_buffer, m_count, count);
            m_count += count;
        }

        public void Clear()
        {
            m_count = 0;
        }

        public int GetCount()
        {
            return m_count;
        }

        public byte[] GetRawBuffer()
        {
            return m_buffer;
        }
    }
}
