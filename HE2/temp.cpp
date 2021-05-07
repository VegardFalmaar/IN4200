
    std::cout << "Transfer count and displacement:" << std::endl;
    for (int i=0; i<numprocs; i++)
      std::cout << " " << transf_cnt_arr[i];
    std::cout << std::endl;
    for (int i=0; i<numprocs; i++)
      std::cout << " " << displacement[i];
    std::cout << std::endl;
