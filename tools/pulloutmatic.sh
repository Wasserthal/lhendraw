ls -1 *.pullout.hxx |cut -d. -f 1 |xargs -I{} echo "domenu(pullout_{},sizeof(pullout_{})/sizeof(menuitem),"\""{}"\"");" > ./generated/pulloutmatic.hxx
cat *.pullout.hxx > ./generated/pullout.hxx
