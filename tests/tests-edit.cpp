#include "../lib/include/catch2/catch.hpp"

#include "../src/implementation.h"
#include "../src/util.h"
#include "../../include/core.h"
#include <iostream>
#include <string>
#include <cstdio>

TEST_CASE("test", "[test]")
{
	implementation impl;
	auto result = impl.addQuery(8, "airport deicing airlines", MT_EDIT_DIST, 1);
	REQUIRE(result == EC_SUCCESS);
	impl.matchDocument(48, "http dbpedia resource pope clement xiii http dbpedia ontology abstract pope clement xiii venice march ndash february rome born carlo della torre rezzonico pope from july february born recently ennobled family venice received jesuit education bologna became cardinal deacon nicola carcere previously filled various important posts curia been bishop padua since during tenure bishop padua visited parishes diocgse first bishop that years became pope july same year rezzonico family were celebrating ludovico rezzonico marriage into powerful savorgnan family bought unfinished palace grand canal finished construction carlo pontiff notorious rampant nepotism notwithstanding meekness affability upright moderate character modest fault classical sculptures vatican provided with mass produced leaves generous with extensive private fortune clement xiii pontificate disturbed perpetual contentions respecting pressures suppress jesuits coming from progressive enlightenment circles philosophes france clement xiii placed encyclop alembert diderot index this index effective used previous century more unexpected resistance came less progressive courts spain sicilies portugal reforming minister joseph portugal ndash marquis pombal expelled jesuits from portugal shipped them masse civitavecchia gift pope pombal sent home papal nuncio recalled portuguese ambassador pamphlet titled brief relation which represented jesuits having virtually independent kingdom south america under their sovereignty tyrannising native americans interest insatiable ambition avarice whether completely true damaging jesuit cause france parlement paris with strong upper bourgeois background jansenist sympathies opened pressure expel jesuits from france spring published excerpts from jesuit writings extrait assertions provided anti jesuit ammunition though arguably many statements extrait contained were made look worse than they were through judicious omission context though congregation bishops assembled paris december recommended action louis france ndash promulgated royal order permitting society remain kingdom with proviso that certain essentially liberalising changes their institution satisfy parlement with french jesuit vicar general should independent general rome august which parlement suppressed jesuits france imposing untenable conditions wishing remain country clement xiii replied protest against invasion church rights annulled eats louis ministers could permit such abrogation french king finally expelled jesuits november papal styles ofen pope clement xiii file clemente xiii reference style holiness spoken style your holiness religious style holy father posthumous style none clement xiii warmly espoused order papal bull apostolicum pascendi january which dismissed criticisms jesuits calumnies praised order usefulness largely ignored jesuits been expelled from france sicilies parma spain they appeared safe charles spain ndash aware drawn contentions bourbon france decided more peremptory efficiency during night april jesuit houses spain were suddenly surrounded inhabitants arrested shipped ports clothes they were wearing bundled onto ships civitavecchia king letter clement xiii promised that allowance piastres each year would withdrawn whole order should them venture time write anything self defence criticism motives expulsion motives that refused discuss then future much same fate awaited them territories bourbon duke parma piacenza advised liberal minister guillaume tillot clement xiii issued strong protest monitorium against policy parmese government question investiture parma aggravated pope troubles bourbon kings espoused their relative quarrel seized avignon benevento pontecorvo united peremptory demand total suppression jesuits january driven extremes clement xiii consented call consistory consider step very meeting died february without suspicion poison which however there appears conclusive evidence from annual register pope clement xiii honestest world most exemplary ecclesiastic purest morals devout steady learned diligent http wikipedia wiki pope clement xiii");
	DocID p_doc_id;
	unsigned int p_num_res;
	QueryID *p_query_ids;

	REQUIRE(impl.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_num_res == 0);
	free(p_query_ids);
}

// TEST_CASE("test2", "[test2]")
// {
// 	StartQuery(2, "diocese pgdma", MT_HAMMING_DIST, 2);
// 	auto x = fopen("../test_data/temp.txt", "r");
// 	char t[100000];
// 	fgets(t, 100000, x);
// 	MatchDocument(22, t);
// 	unsigned int doc_id = 0;
// 	unsigned int num_res = 0;
// 	unsigned int *query_ids = 0;

// 	ErrorCode err = GetNextAvailRes(&doc_id, &num_res, &query_ids);
// 	REQUIRE(err == EC_SUCCESS);
// 	REQUIRE(num_res == 0);
// 	free(query_ids);
// }

TEST_CASE("test1", "[test1]")
{
	implementation impl;
	auto result = impl.addQuery(1, "tesa is a test", MT_EXACT_MATCH, 0);
	REQUIRE(result == EC_SUCCESS);
	impl.removeQuery(1);
	REQUIRE(result == EC_SUCCESS);
}
