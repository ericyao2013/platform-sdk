class Airmapd < Formula
  homepage "https://github.com/airmap/airmapd"
  url "https://github.com/airmap/airmapd.git"
  version "0.0.1"

  depends_on "cmake" => :build
  depends_on "boost"
  depends_on "pkgconfig"
  depends_on "protobuf"
  depends_on "openssl"

  def install
    Dir.mkdir("build")
    Dir.chdir("build")
    system "cmake","-DCMAKE_INSTALL_PREFIX=#{prefix}", ".."
    system "make"
    system "make", "install"
  end

end
