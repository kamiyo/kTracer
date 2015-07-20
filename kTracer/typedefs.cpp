//int height = (int) samples.rows(), width = (int) samples.cols();
//for (int j = 0; j < height; j++) {
//	for (int i = 0; i < width; i++) {
//		samples(j, i
//	}
//}
//
//void genPoints(Sampler2d& samples) {
//	for (int j = 0; j < m_height; j++) {
//		for (int i = 0; i < m_width; i++) {
//			m_image(j, i) = Vector2d((i + (j + m_rng->real(0, 1)) / (double) m_height) / (double) m_width, (j + (i + m_rng->real(0, 1)) / (double) m_width) / (double) m_height);
//			m_lens(j, i) = Vector2d((i + (j + m_rng->real(0, 1)) / (double) m_height) / (double) m_width, (j + (i + m_rng->real(0, 1)) / (double) m_width) / (double) m_height);
//		}
//	}
//	shuffle_correlated(m_image, m_rng);
//	shuffle_correlated(m_lens, m_rng);
//	shuffle(m_lens, m_rng, true);
//}