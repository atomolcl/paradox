#ifndef RINGBUFFER_H
#define RINGBUFFER_H

namespace paradox {
	/*not threadsafe and default alignment is 4*/
	class ringbuffer {
	public:
		explicit ringbuffer(size_t size);
		~ringbuffer();

		void initialize();

		/*for boost interface*/
		char* volatile get_read_point() const { return m_read_point; }
		char* volatile get_write_point() const { return m_write_point; }
		char* get_buffer_begin() const { return m_buffer_begin; }
		char* get_buffer_end() const { return m_buffer_end; }


		/*raw interface*/
		void write(const char* souce, size_t size);

		/*@input @size is try to read size
			rewrite @size to actually read size
			@read is already read buffer
			@return return false means no more date to read
		*/
		bool begin_read(char*& read, size_t& size);

		void finish_read(size_t read_size);
	private:
		char* m_buffer_begin;
		char* m_buffer_end;
		char* volatile m_write_point;
		char* volatile m_read_point;
	};
}

#endif