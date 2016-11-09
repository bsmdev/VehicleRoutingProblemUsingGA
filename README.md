# Vehicle Routing Problem Using Genetic Algorithm
Tugas Program 2 - Artificial Intelligence - Implementasi Genetic Algorithm
<br/>
<br/>
<br/>
<br/>
Deskripsi Kasus : <br/>
Vehicle Routing Problem (VRP) merupakan sebuah permasalahan di mana terdapat beberapa rute yang harus dilalui oleh sejumlah kendaraan yang berangkat dari suatu depot menuju beberapa tempat tujuan yang telah ditentukan dan berakhir pada depot yang sama. Salah satu pengembangan dari kasus VRP adalah dengan menambahkan batasan kapasitas pada kendaraan yang mengangkut, sehingga disebut sebagai Capacitated VRP. Contoh implementasi permasalahan Capacitated VRP tersebut ialah pendistribusian barang dari sebuah perusahaan ke beberapa customer, dimana kendaraan yang digunakan hanya dapat menampung dengan kapasitas berat tertentu, sehingga jika berat barang sudah melebihi kapasitas berat kendaraan, maka kendaraan tersebut harus kembali ke depot untuk mengambil barang-barang customer yang lainnya. Setiap customer memiliki demand (berat barang) masing-masing yang harus dipenuhi. Jumlah kendaraan yang digunakan dapat bervariasi. Namun, untuk kasus yang paling sederhana yang akan digunakan pada Tugas Program ini, hanya terdapat satu kendaraan. Permasalahan VRP ini dapat dikerjakan dengan Algoritma Genetika karena permasalahan tersebut merupakan permasalahan kombinatorial.<br/>
<br/>
Berdasarkan kasus tersebut, dimintalah sebuah program dengan menggunakan Metode Genetic Algorithm yang dapat mengeluarkan hasil solusi rute yang harus dilalui kendaraan supaya semua demands dari customer dapat didistribusikan dengan cost yang seminimum mungkin. Cost dapat dihitung dari total jarak yang dilalui oleh kendaraan untuk mendistribusikan semua barang (demand) ke seluruh customer dan kembali lagi ke depot.
