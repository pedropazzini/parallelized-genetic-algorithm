##################################################################

define veclen
	set $veclen = $arg0.size()
end

document veclen
Seta o tamnaho do conteiner na variavel veclen
end

##################################################################

define veclen_coord
	set $veclen_coord = $arg0.size()
end

document veclen_coord
Seta o tamnaho do conteiner na variavel veclen
end

##################################################################


define pveclen

	veclen $arg0
	output $veclen
	echo \n
end

##################################################################

define pveci
	output **($arg0._M_impl._M_start + $arg1)
  	echo \n
end

##################################################################

define pvec
	veclen $arg0
	set $lloop_pvec = 0
	while ($lloop_pvec < $veclen)
  		pveci $arg0 $lloop_pvec
		set $lloop_pvec = $lloop_pvec +1
	end
end

##################################################################

define pveci_no_pt
	output *($arg0._M_impl._M_start + $arg1)
  	echo \n
end

##################################################################

define pvec_no_pt
	veclen $arg0
	set $lloop_pvec_no_pt = 0
	while ($lloop_pvec_no_pt< $veclen)
  		pveci_no_pt $arg0 $lloop_pvec_no_pt
		set $lloop_pvec_no_pt = $lloop_pvec_no_pt +1
	end
end

##################################################################

define ppopi
	echo (
	output (*($arg0._M_impl._M_start+$arg1))
	echo ;
	output (*($arg0._M_impl._M_start+$arg1))->_pair
	echo )
	echo _
	output (*($arg0._M_impl._M_start+$arg1))->_identifier
	echo -> 
	output (*($arg0._M_impl._M_start+$arg1))->_val
	echo ' '
	ppos (*($arg0._M_impl._M_start+$arg1))->_position
end

document ppopi
Imprime o identificador o valor e as coordendas de um individuo
end

##################################################################

define ppop
	veclen $arg0
	set $lloop_ppop = 0
	while ($lloop_ppop < $veclen)
  		ppopi $arg0 $lloop_ppop
		set $lloop_ppop = $lloop_ppop + 1
	end
	echo \n
end

document ppop
Imprime todas as características dos indivíduos de uma população
end

##################################################################

define pposi
	output (*($arg0._M_impl._M_start+$arg1))->_value
	echo ' '
	p/t (*($arg0._M_impl._M_start+$arg1))->_value
end

document pposi
Imprime uma coordenada da posicao do individuo(valor não codificado)
end

##################################################################

define ppos
	veclen_coord $arg0
	set $lloop_ppos = 0
	echo (
	while ($lloop_ppos < $veclen_coord)
  		pposi $arg0 $lloop_ppos
		set $lloop_ppos = $lloop_ppos +1
	end
end

document ppos
Imprime a posicao de um individuo na tela (valores não codificados)
end

#################################################################

define prouli
	echo (
	output (($arg0._M_impl._M_start+$arg1)->first)
	echo ,
	output (($arg0._M_impl._M_start+$arg1)->second)
	echo )
	echo \n
end

document prouli
Imprime um campo da roleta do metodo de seleco
end

#################################################################

define proul
	veclen $arg0
	set $loop_proul = 0
	while ($loop_proul < $veclen)
  		prouli $arg0 $loop_proul
		set $loop_proul = $loop_proul +1
	end
end

document proul
Imprime toda a roleta do método de selecao por roleta
end

#################################################################

