/* libstocks_client - client of the libstocks library
 * Copyright (C) 2000 Eric Laeuffer
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stocks.h"

void Usage(void)
{
  printf("Usage:\n");
  printf("To get stocks quotes:\n");
  printf("libstocks \"symbol1+symbol2\"\n");
  printf("exemple > libstocks_client \"^IXIC+35000.PA+BOBJ\"\n");
  printf("To get stock historical quotes:\n");
  printf("libstocks symbol first_date last_date\n");
  printf("exemple > libstocks_client BOBJ 2000/01/01 2000/08/01\n");
}

void DefProxy(void)
{
  char *proxy;
  libstocks_return_code error;

  /* Proxy support */
  /* Checks for "http_proxy" environment variable */
  proxy = getenv("http_proxy");
  if(proxy)
    {
      /* printf("proxy set\n"); */
      error = set_proxy(proxy);
      if (error) 
	{
	  printf("Proxy error (%d)\n", error);
	  exit(1);
	}
    }
}

void GetHistorical(char *symbol, char *date1, char *date2)
{
  stock *stocks_quotes=NULL;
  stock *stocks_tmp;

  libstocks_return_code error;

  DefProxy();

  error = get_stock_history(symbol, date1, date2, &stocks_quotes);
  if (error)
    {
      printf("Error in getting stocks (%d)\n", error);
      exit(1);
    }

  stocks_tmp = stocks_quotes;

  /* Displays the stocks */
  while(stocks_tmp!=0)
    {
      printf("%s,", stocks_tmp->Date);
      printf("%8.3f,", stocks_tmp->OpenPrice);
      printf("%8.3f,", stocks_tmp->MaxPrice);
      printf("%8.3f,", stocks_tmp->MinPrice);
      printf("%8.3f,", stocks_tmp->LastPrice);
      printf("%d\n", stocks_tmp->Volume);

      /* Simple function which help to browse in the stocks list */ 
      stocks_tmp = next_stock(stocks_tmp);
    }

  /* frees stocks */
  free_stocks(stocks_quotes);

}

void GetStocks(char *stock_liste)
{
  stock *stocks_quotes=NULL;
  stock *stocks_tmp;

  libstocks_return_code error;

  DefProxy();

  /* Get the stocks and process errors */
  error = get_stocks(stock_liste, &stocks_quotes);
  if (error)
    {
      printf("Error in getting stocks (%d)\n", error);
      exit(1);
    }

  stocks_tmp = stocks_quotes;

  /* Displays the stocks */
  while(stocks_tmp!=0)
    {

      if (stocks_tmp->Time)
	{
	  printf("%s ", stocks_tmp->Time);
	}
      if (stocks_tmp->Date)
	{
	  printf("%s ", stocks_tmp->Date);
	}
      printf("\n");
      
      printf("----------------------------------------\n");

      if ( strlen(stocks_tmp->Symbol) > 20 ) 
	printf("| Symbol    | %.20s     |\n",stocks_tmp->Symbol);
      else printf("| Symbol    | %-20s     |\n",stocks_tmp->Symbol);

      if (stocks_tmp->Name)
	{
	  if ( strlen(stocks_tmp->Name) > 20 ) 
	    printf("| Name      | %.20s     |\n",stocks_tmp->Name);
	  else printf("| Name      | %-20s     |\n",stocks_tmp->Name); 
	}
      else printf("| Name      |                          |\n"); 

      printf("| Price     | %-7.2f                  |\n", 
	     stocks_tmp->CurrentPrice);
      printf("| Yesterday | %-7.2f                  |\n", 
	     stocks_tmp->LastPrice);
      printf("| Open      | %-7.2f                  |\n", 
	     stocks_tmp->OpenPrice);
      printf("| Min       | %-7.2f                  |\n", 
	     stocks_tmp->MinPrice);
      printf("| Max       | %-7.2f                  |\n", 
	     stocks_tmp->MaxPrice);
      printf("| Var       | %-6.2f (%5.2f %%)         |\n", 
	     stocks_tmp->Variation, stocks_tmp->Pourcentage);
      printf("| Volume    | %-9d                |\n", 
	     stocks_tmp->Volume);
      printf("----------------------------------------\n\n");

      /* Simple function which help to browse in the stocks list */ 
      stocks_tmp = next_stock(stocks_tmp);
    }

  /* frees stocks */
  free_stocks(stocks_quotes);

}

int main(int argc,char **argv) 
{

  /* Args support */
  if (argc==2)
    {
      GetStocks(argv[1]);
    }
  else if (argc==4)
    {
      GetHistorical(argv[1], argv[2], argv[3]);
    }
  else
    {
      Usage();
    }

  return(0);
}
