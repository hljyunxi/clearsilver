
#include <unistd.h>
#include <neo_hdf.h>
#include <neo_test.h>

int rand_name (char *s, int slen)
{
  char buf[256];
  int x, m, l;

  m = neot_rand(10);
  neot_rand_word(s, slen);

  for (x = 1; x < m; x++)
  {
    l = strlen(s);
    neot_rand_word(buf, sizeof(buf));
    snprintf(s + l, slen - l, ".%s", buf);
  }

  return 0;
}

static int sortByName(const void *a, const void *b) {
  HDF **ha = (HDF **)a;
  HDF **hb = (HDF **)b;

  /* fprintf(stderr, "%s <=> %s\n", hdf_obj_name(*ha), hdf_obj_name(*hb));  */
  return strcasecmp(hdf_obj_name(*ha), hdf_obj_name(*hb));
}


int main(int argc, char *argv[])
{
  NEOERR *err;
  HDF *hdf;
  int x;
  char name[256];
  char value[256];
  double tstart = 0;

  err = hdf_init(&hdf);
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }

  err = hdf_set_value (hdf, "Beware", "1");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  err = hdf_set_value (hdf, "Beware.The", "2");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  err = hdf_set_value (hdf, "Beware.The.Ides", "3");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  err = hdf_set_value (hdf, "Beware.Off", "4");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  err = hdf_set_value (hdf, "Beware.The.Ides.Of", "5");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  err = hdf_set_value (hdf, "Beware.The.Butter", "6");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  err = hdf_set_attr (hdf, "Beware.The.Butter", "Lang", "en");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  err = hdf_set_attr (hdf, "Beware.The.Butter", "Lang", "1");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  err = hdf_set_attr (hdf, "Beware.The.Butter", "Lang", NULL);
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }

  err = hdf_read_file (hdf, "test.hdf");
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  hdf_dump(hdf, NULL);


  x = hdf_get_int_value (hdf, "Beware.The.Ides", 0);
  if (err != STATUS_OK) 
  {
    nerr_log_error(err);
    return -1;
  }
  if (x != 3)
  {
    ne_warn("hdf_get_int_value returned %d, expected 3", x);
    return -1;
  } 

  for (x = 0; x < 10000; x++)
  {
    rand_name(name, sizeof(name));
    neot_rand_word(value, sizeof(value));
    /* ne_warn("Setting %s = %s", name, value); */
    err = hdf_set_value (hdf, name, value);
    if (err != STATUS_OK) 
    {
      nerr_log_error(err);
      return -1;
    }
  }

  tstart = ne_timef();
  hdf_sort_obj(hdf, sortByName);
  ne_warn("sort took %5.5fs", ne_timef() - tstart);

  /* hdf_dump(hdf, NULL); */

  hdf_destroy(&hdf);

  return 0;
}
