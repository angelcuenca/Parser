/** Copyright (c) 2013 Angel Sanchez 
 *  angelscrf@gmail.com
 *  University of Guadalajara - Project - Parser RSS
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the Software), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
*/

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
fstream ficheroEntrada;

class Lexico{

public:
	bool continua;
	bool noSiguienteCaracter;
	bool contenido;
	int estado;
	int tipo;
	char c;
	string cadena;

	int analisisLexico( )
	{
		continua=true;
		contenido=false;
		estado=0;
		cadena="";

		while( continua )
		{
			if( !noSiguienteCaracter )
				c=aperturaArchivo();

			noSiguienteCaracter=false;

			switch( estado )
			{
				case 0:	if( c == '<' )
							estado=3;
						else if( (c >= 33 && c <= 126) )
							sigEstado(1);
						else if( c == ' ' || c == '\n' || c == '\t' )
							estado=0;
						else if( ficheroEntrada.eof() )
							continua=false;
						/*	else
						{
							estadoAceptacion(0);
							noSiguienteCaracter=true;
						} */
						break;

				//Contenido entre las etiquetas <> </>
				case 1:  while( c != '<' && !ficheroEntrada.eof() )
						{
							sigEstado(1);
							c=aperturaArchivo();
	
							if( c == '<' )
							{
								contenido=true;
								noSiguienteCaracter=true;
							}
	
						}
	
						if( contenido == true )
						{
							estadoAceptacion(1);
						}
						else
						{
							estadoAceptacion(0);
						}
						break;

				case 3:	if( c == ' ' || c == '\t' )
						{
							estado=3;
						}
						else if( (c >= 65 && c <= 90) || (c >= 97 && c <= 122 ) )
							sigEstado(4);
						else if( c == '/' )
							estado=5;
						else
						{
							estadoAceptacion(0);
						}
						break;

				//Estados etiqueta apertura
				case 4:	if( (c >= 65 && c <= 90) || (c >= 97 && c <= 122 ) )
							sigEstado(4);
						else if( c == ' ' || c == '\t' )
						{
							tipo=revisaEtiqueta();
							if( tipo == 4 )
							{
								bool fin;
								while( c != '>' && !ficheroEntrada.eof() )
								{
									c=aperturaArchivo();
		
									if( c == '>' )
										fin=true;
								}
		
								if( fin == true )
									estadoAceptacion( tipo );
								else
									estadoAceptacion(0);
							}
							else if( tipo == 5 || tipo == 6 || tipo == 7 ||
										tipo == 8 || tipo == 9 )
							{
								bool fin;
								while( c != '>' && !ficheroEntrada.eof() )
								{
									c=aperturaArchivo();
		
									if( c == '>' )
										fin=true;
									else if( c != ' ' && c != '\t' )
									{
										c='>';
										fin=false;
									}
		
								}
		
								if( fin == true )
								{
									estadoAceptacion( tipo );
								}
								else
									estadoAceptacion(0);
							}
							else
							{
								estadoAceptacion(0);
							}
						}
						else if( c == '>' )
						{
							tipo=revisaEtiqueta();
							if( tipo == 4 )
							{
								estadoAceptacion( tipo );
							}
							else if( tipo == 5 || tipo == 6 || tipo == 7 ||
										tipo == 8 || tipo == 9 )
							{
								estadoAceptacion( tipo );
							}
		
						}
						else
						{
							estadoAceptacion(0);
						}
						break;


				case 5:	if( c == ' ' || c == '\t' )
						{
							estado=5;
						}
						else if( (c >= 65 && c <= 90) || (c >= 97 && c <= 122 ) )
							sigEstado(6);
						else
						{
							estadoAceptacion(0);
						}
						break;

							//Estados etiqueta cerradura
				case 6:	if( (c >= 65 && c <= 90) || (c >= 97 && c <= 122 ) )
							sigEstado(6);
						else if( c == ' ' || c == '\t' )
						{
							tipo=revisaEtiqueta();
							if( tipo == 4 )
							{
								bool fin;
								while( c != '>' && !ficheroEntrada.eof() )
								{
									c=aperturaArchivo();

									if( c == '>' )
										fin=true;
									else if( c != ' ' && c != '\t' )
									{
										c='>';
										fin=false;
									}

								}

								if( fin == true )
									estadoAceptacion( 10 );
								else
									estadoAceptacion(0);
							}
							else if( tipo == 5 || tipo == 6 || tipo == 7 ||
										tipo == 8 || tipo == 9 )
							{
								bool fin;
								while( c != '>' && !ficheroEntrada.eof() )
								{
									c=aperturaArchivo();

									if( c == '>' )
										fin=true;
									else if( c != ' ' && c != '\t' )
									{
										c='>';
										fin=false;
									}

								}

								if( fin == true )
								{
									switch( tipo )
									{
										case 5:  estadoAceptacion( 11 );
													break;
										case 6:	estadoAceptacion( 12 );
													break;
										case 7:  estadoAceptacion( 13 );
													break;
										case 8: 	estadoAceptacion( 15 );
													break;
										case 9: 	estadoAceptacion( 14 );
													break;
									}
								}
								else
									estadoAceptacion(0);
							}
							else
							{
								estadoAceptacion(0);
							}
						}
						else if( c == '>' )
						{
							tipo=revisaEtiqueta();
							if( tipo == 4 )
							{
								estadoAceptacion( 10 );
							}
							else if( tipo == 5 || tipo == 6 || tipo == 7 ||
										tipo == 8 || tipo == 9 )
							{
									switch( tipo )
									{
										case 5:  estadoAceptacion( 11 );
													break;
										case 6:	estadoAceptacion( 12 );
													break;
										case 7:  estadoAceptacion( 13 );
													break;
										case 8: 	estadoAceptacion( 15 );
													break;
										case 9: 	estadoAceptacion( 14 );
													break;
									}
							}
							else
							{
								estadoAceptacion(0);
							}
						}
						else
						{
							estadoAceptacion(0);
						}
						break;

			} //fin switch

		} //fin while

		return estado;
	}	//fin analisisLexico()

	void sigEstado( int n )
	{
		estado=n;

      if( n != 0)
			cadena+=c;
	}

   void estadoAceptacion( int n )
   {
		estado=n;
      continua=false;
	}

	int revisaEtiqueta( )
	{
		if( cadena      == "rss" )
			return 4;
		else if( cadena == "channel" )
         return 5;
      else if( cadena == "title" )
         return 6;
      else if( cadena == "link" )
         return 7;
		else if( cadena == "item" )
         return 8;
		else if( cadena == "description" )
         return 9;
      else
         return 1;
	}

	char aperturaArchivo( )
	{
      if ( ! ficheroEntrada.eof() )
      {
          char letra = ficheroEntrada.get();
          return letra;
      }

      return '$';
   }

}; //class Lexico


class Sintactico{
public:
	bool tipo;

	void analisisSintactico( )
	{
		ofstream myfile2 ("salida.txt");
		if ( myfile2.is_open() )
		{
			myfile2 <<"";

			myfile2.close();
		}

		Lexico lexico;
		ficheroEntrada.open("entrada.txt", ios::in);

		if( ficheroEntrada.is_open() )
		{
			tipo=inicio( lexico );
			if( tipo != true )
			{
			//	cout<< "Error";
				creacionSalida("Error\n");
			}


			ficheroEntrada.close();
		}
		else
			cout << "Fichero entrada.txt inexistente" << endl;
	}

	//Etiqueta rss
	bool inicio( Lexico lexico  )
	{
		lexico.analisisLexico();

		if( lexico.estado == 4 )
		{
			if( c(lexico) == true )
			{
				lexico.estado=10;
				if( lexico.estado == 10 )
				{
					return true;
				}
			}
		}

		return false;
	}

	//Etiqueta channel
	bool c( Lexico lexico )
	{
		lexico.analisisLexico();

		if( lexico.estado == 5 )
		{
			creacionSalida("Inicio canal\n");
			if( o(lexico) == true )
			{
				if( i(lexico) == true )
				{  lexico.estado=11;
					if( lexico.estado == 11 )
					{
						creacionSalida("Fin canal\n");
						lexico.analisisLexico();
						if( lexico.estado == 10 )
							return true;
						else if( c2(lexico) == true )
							return true;
					}
				}
			}

		}

		return false;
	}

	//Etiqueta channel
	bool c2( Lexico lexico )
	{
		if( lexico.estado == 5 )
		 {
			  creacionSalida("Inicio canal\n");
			  if( o(lexico) == true )
			  {
					if( i(lexico) == true )
					{
						lexico.estado=11;
						 if( lexico.estado == 11 )
						 {
							  creacionSalida("Fin canal\n");
							  lexico.analisisLexico(); //cout << lexico.estado;
							  if( lexico.estado == 10 )
							  {
									return true;
							  }
							  else if( c2(lexico) == true )
							  {
									return true;
							  }
						 }
					}
			  }
		 }

		 return false;
	}

	//Etiqueta
	bool o( Lexico lexico)
	{
		if( t(lexico) == true )
		{
			if( l(lexico) == true)
			{
				if( d(lexico) == true)
				{
					return true;
				}
			}
		}

		return false;
	}

	//Etiqueta title
	bool t( Lexico lexico )
	{
		 lexico.analisisLexico();
		 if( lexico.estado == 6 )
		 {
			  lexico.analisisLexico();
			  if( lexico.estado == 1 )
			  {
					creacionSalida("Titulo: "+lexico.cadena+"\n");
					lexico.analisisLexico();
					if( lexico.estado == 12 )
					{
						 return true;
					}
			  }
		 }
		 return false;
	}

	//Etiqueta  link
	bool l( Lexico lexico )
	{
		lexico.analisisLexico();
		 if( lexico.estado == 7 )
		 {
			  lexico.analisisLexico();
			  if( lexico.estado == 1 )
			  {
				  creacionSalida("Enlace: "+lexico.cadena+"\n");
				  lexico.analisisLexico();
					if( lexico.estado == 13 )
					{
						 return true;
					}
			  }
		 }
		 return false;
	}

	//Etiqueta description
	bool d( Lexico lexico )
	{
		lexico.analisisLexico();
		 if( lexico.estado == 9 )
		 {
			  lexico.analisisLexico();
			  if( lexico.estado == 1 )
			  {
				 creacionSalida("Descripcion: "+lexico.cadena+"\n");
				 lexico.analisisLexico();
					if( lexico.estado == 14 )
					{
						 return true;
					}
			  }
		 }
		 return false;
	}

	//Etiqueta item
	bool i( Lexico lexico )
	{
		lexico.analisisLexico();

		 if( lexico.estado == 8 )
		 {
			  creacionSalida("Inicio elemento\n");

			  if( o(lexico) == true )
			  {
					lexico.analisisLexico();
					if( lexico.estado == 15 )
					{
						 creacionSalida("Fin elemento\n");
						 lexico.analisisLexico();
						 if( lexico.estado == 11 )
						 {
							  return true;
						 }
						 else if( i2(lexico) == true )
						 {
							  return true;
						 }
					}
			  }
		 }
		return false;
	}

	bool i2( Lexico lexico )
	{
		if( lexico.estado == 8 )
		{
			  creacionSalida("Inicio elemento\n");
			  if( o(lexico) == true )
			  {
					lexico.analisisLexico();
					if( lexico.estado == 15 )
					{
						 creacionSalida("Fin elemento\n");
						 lexico.analisisLexico();
						 if( lexico.estado == 11 )
						 {
							  return true;
						 }
						 else if( i2(lexico) == true )
						 {
							  return true;
						 }
					}
			  }
		 }
		 return false;
	}

	void creacionSalida( string primero )
	{
		 ofstream myfile2 ("salida.txt",ios :: app);
		 if (myfile2.is_open() )
		 {
			  myfile2 << primero ;

			  myfile2.close();
		 }
		 else
			  cout << "Error al generar archivo salida" << endl;

	}

};

int main()
{
	Sintactico sintactico;

	sintactico.analisisSintactico();

   return 0;
}
