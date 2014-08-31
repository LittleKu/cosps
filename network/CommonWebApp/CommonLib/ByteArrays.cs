using System;
using System.IO;
using System.Collections.Generic;

namespace CommonLib
{
    public class ByteArrays
    {
        private List<RangeArray> m_byteArrays = new List<RangeArray>();

        public List<RangeArray> ByteArrayList
        {
            get
            {
                return m_byteArrays;
            }

            set
            {
                m_byteArrays = value;
            }
        }

        public void AddByteArray(byte[] bytes)
        {
            RangeArray ra = new RangeArray();
            ra.Buffer = bytes;
            ra.Offset = 0;
            ra.Count = bytes.Length;
            m_byteArrays.Add(ra);
        }

        public void AddByteArray(byte[] bytes, int offset, int count)
        {
            RangeArray ra = new RangeArray();
            ra.Buffer = bytes;
            ra.Offset = offset;
            ra.Count = count;
            m_byteArrays.Add(ra);
        }

        public void Write(Stream stream)
        {
            if (m_byteArrays.Count <= 0)
            {
                return;
            }

            byte[] buffer = null;

            //length of RangeArray
            buffer = BitConverter.GetBytes(m_byteArrays.Count);
            stream.Write(buffer, 0, buffer.Length);

            for (int i = 0; i < m_byteArrays.Count; i++)
            {
                //length of the current array
                buffer = BitConverter.GetBytes(m_byteArrays[i].Count);
                stream.Write(buffer, 0, buffer.Length);

                //content of the current array
                stream.Write(m_byteArrays[i].Buffer, m_byteArrays[i].Offset, m_byteArrays[i].Count);
            }
        }

        public void Read(byte[] buffer)
        {
            if (buffer == null || buffer.Length <= 0)
            {
                return;
            }

            int index = 0, currCount;

            int numOfArray = BitConverter.ToInt32(buffer, index);
            index += 4;

            for (int i = 0; i < numOfArray; i++)
            {
                currCount = BitConverter.ToInt32(buffer, index);
                index += 4;

                RangeArray ra = new RangeArray();
                ra.Buffer = buffer;
                ra.Offset = index;
                ra.Count = currCount;
                m_byteArrays.Add(ra);

                index += currCount;
            }
        }
    }

    public class RangeArray
    {
        public byte[] Buffer { get; set; }

        public int Offset { get; set; }

        public int Count { get; set; }
    }
}
