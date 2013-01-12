 * Copyright (C) the libgit2 contributors. All rights reserved.
		ctxt->diff->strcomp(delta->new_file.path, delta->old_file.path) == 0);
	diff_context *ctxt,
	git_diff_delta *delta,
	git_diff_file *file,
	const git_map *map)
	const git_buf search = { map->data, 0, min(map->len, 4000) };
		/* TODO: provide encoding / binary detection callbacks that can
		 * be UTF-8 aware, etc.  For now, instead of trying to be smart,
		 * let's just use the simple NUL-byte detection that core git uses.
		 */
		/* previously was: if (git_buf_text_is_binary(&search)) */
		if (git_buf_text_contains_nul(&search))
	map->len  = (size_t)git_blob_rawsize(*blob);
		if ((sm_head = git_submodule_wd_id(sm)) != NULL ||
			(sm_head = git_submodule_head_id(sm)) != NULL)
			if (!file->size)
				goto close_and_cleanup;

	git_diff_file_cb file_cb,
	git_diff_hunk_cb hunk_cb,
	git_diff_data_cb data_cb,
	void *payload)
	ctxt->payload = payload;
	ctxt->error = 0;
	if (ctxt->file_cb(delta, progress, ctxt->payload) != 0)
		ctxt->error = GIT_EUSER;
	return ctxt->error;
		ctxt->error = parse_hunk_header(&ctxt->range, bufs[0].ptr);
		if (ctxt->error < 0)
			return ctxt->error;
			ctxt->hunk_cb(patch->delta, &ctxt->range,
				bufs[0].ptr, bufs[0].size, ctxt->payload))
			ctxt->error = GIT_EUSER;
			ctxt->data_cb(patch->delta, &ctxt->range,
				origin, bufs[1].ptr, bufs[1].size, ctxt->payload))
			ctxt->error = GIT_EUSER;
	if (len == 3 && !ctxt->error) {
			ctxt->data_cb(patch->delta, &ctxt->range,
				origin, bufs[2].ptr, bufs[2].size, ctxt->payload))
			ctxt->error = GIT_EUSER;
	return ctxt->error;
	error = ctxt->error;
	size_t header_len,
	void *payload)
	git_diff_patch *patch = payload;
	patch->oldno = range->old_start;
	patch->newno = range->new_start;

	size_t content_len,
	void *payload)
	git_diff_patch *patch = payload;
	switch (line_origin) {
	case GIT_DIFF_LINE_ADDITION:
		line->oldno = -1;
		line->newno = patch->newno;
		patch->newno += line->lines;
		break;
	case GIT_DIFF_LINE_DELETION:
		line->oldno = patch->oldno;
		line->newno = -1;
		patch->oldno += line->lines;
		break;
	default:
		line->oldno = patch->oldno;
		line->newno = patch->newno;
		patch->oldno += line->lines;
		patch->newno += line->lines;
		break;
	git_diff_file_cb file_cb,
	git_diff_hunk_cb hunk_cb,
	git_diff_data_cb data_cb,
	void *payload)
		file_cb, hunk_cb, data_cb, payload);
	git_diff_data_cb print_cb;
	void *payload;
	const git_diff_delta *delta, float progress, void *data)
		pi->diff->strcomp(delta->old_file.path,delta->new_file.path) != 0)
	if (pi->print_cb(delta, NULL, GIT_DIFF_LINE_FILE_HDR,
			git_buf_cstr(pi->buf), git_buf_len(pi->buf), pi->payload))
	git_diff_data_cb print_cb,
	void *payload)
	pi.payload  = payload;
	error = git_diff_foreach(diff, print_compact, NULL, NULL, &pi);
	const git_diff_delta *delta, float progress, void *data)
	if (pi->print_cb(delta, NULL, GIT_DIFF_LINE_FILE_HDR,
			git_buf_cstr(pi->buf), git_buf_len(pi->buf), pi->payload))
	if (pi->print_cb(delta, NULL, GIT_DIFF_LINE_BINARY,
			git_buf_cstr(pi->buf), git_buf_len(pi->buf), pi->payload))
	size_t header_len,
	void *data)
	if (pi->print_cb(d, r, GIT_DIFF_LINE_HUNK_HDR,
			git_buf_cstr(pi->buf), git_buf_len(pi->buf), pi->payload))
	size_t content_len,
	void *data)
	if (pi->print_cb(delta, range, line_origin,
			git_buf_cstr(pi->buf), git_buf_len(pi->buf), pi->payload))
	git_diff_data_cb print_cb,
	void *payload)
	pi.payload  = payload;
		diff, print_patch_file, print_patch_hunk, print_patch_line, &pi);
	const git_blob *blob, git_map *map, git_diff_file *file)
		file->size = git_blob_rawsize(blob);

		map->len   = (size_t)file->size;
		map->data  = (char *)git_blob_rawcontent(blob);
		file->size = 0;

		map->len   = 0;
		map->data  = "";
static void set_data_from_buffer(
	const char *buffer, size_t buffer_len, git_map *map, git_diff_file *file)
	file->size = (git_off_t)buffer_len;
	file->mode = 0644;

	if (!buffer)
		file->flags |= GIT_DIFF_FILE_NO_DATA;
	else
		git_odb_hash(&file->oid, buffer, buffer_len, GIT_OBJ_BLOB);

	map->len   = buffer_len;
	map->data  = (char *)buffer;
}

typedef struct {
	diff_context   ctxt;
} diff_single_data;
static int diff_single_init(
	diff_single_data *data,
	git_repository *repo,
	const git_diff_options *opts,
	git_diff_file_cb file_cb,
	git_diff_hunk_cb hunk_cb,
	git_diff_data_cb data_cb,
	void *payload)
{
	GITERR_CHECK_VERSION(opts, GIT_DIFF_OPTIONS_VERSION, "git_diff_options");
	memset(data, 0, sizeof(*data));
		&data->ctxt, NULL, repo, opts, file_cb, hunk_cb, data_cb, payload);
	diff_patch_init(&data->ctxt, &data->patch);
	return 0;
}
static int diff_single_apply(diff_single_data *data)
{
	int error;
	git_diff_delta *delta = &data->delta;
	bool has_old = ((delta->old_file.flags & GIT_DIFF_FILE_NO_DATA) == 0);
	bool has_new = ((delta->new_file.flags & GIT_DIFF_FILE_NO_DATA) == 0);
	/* finish setting up fake git_diff_delta record and loaded data */
	data->patch.delta = delta;
	delta->binary = -1;
	delta->status = has_new ?
		(has_old ? GIT_DELTA_MODIFIED : GIT_DELTA_ADDED) :
		(has_old ? GIT_DELTA_DELETED : GIT_DELTA_UNTRACKED);
	if (git_oid_cmp(&delta->new_file.oid, &delta->old_file.oid) == 0)
		delta->status = GIT_DELTA_UNMODIFIED;
			&data->ctxt, delta, &delta->old_file, &data->patch.old_data)) < 0 ||
			&data->ctxt, delta, &delta->new_file, &data->patch.new_data)) < 0)
	data->patch.flags |= GIT_DIFF_PATCH_LOADED;

	if (delta->binary != 1 && delta->status != GIT_DELTA_UNMODIFIED)
		data->patch.flags |= GIT_DIFF_PATCH_DIFFABLE;
	if (!(error = diff_delta_file_callback(&data->ctxt, delta, 1)))
		error = diff_patch_generate(&data->ctxt, &data->patch);
	diff_patch_unload(&data->patch);

int git_diff_blobs(
	const git_blob *old_blob,
	const git_blob *new_blob,
	const git_diff_options *options,
	git_diff_file_cb file_cb,
	git_diff_hunk_cb hunk_cb,
	git_diff_data_cb data_cb,
	void *payload)
{
	int error;
	diff_single_data d;
	git_repository *repo =
		new_blob ? git_object_owner((const git_object *)new_blob) :
		old_blob ? git_object_owner((const git_object *)old_blob) : NULL;

	if ((error = diff_single_init(
			&d, repo, options, file_cb, hunk_cb, data_cb, payload)) < 0)
		return error;

	if (options && (options->flags & GIT_DIFF_REVERSE) != 0) {
		const git_blob *swap = old_blob;
		old_blob = new_blob;
		new_blob = swap;
	}

	set_data_from_blob(old_blob, &d.patch.old_data, &d.delta.old_file);
	set_data_from_blob(new_blob, &d.patch.new_data, &d.delta.new_file);

	return diff_single_apply(&d);
}

int git_diff_blob_to_buffer(
	const git_blob *old_blob,
	const char *buf,
	size_t buflen,
	const git_diff_options *options,
	git_diff_file_cb file_cb,
	git_diff_hunk_cb hunk_cb,
	git_diff_data_cb data_cb,
	void *payload)
{
	int error;
	diff_single_data d;
	git_repository *repo =
		old_blob ? git_object_owner((const git_object *)old_blob) : NULL;

	if ((error = diff_single_init(
			&d, repo, options, file_cb, hunk_cb, data_cb, payload)) < 0)
		return error;

	if (options && (options->flags & GIT_DIFF_REVERSE) != 0) {
		set_data_from_buffer(buf, buflen, &d.patch.old_data, &d.delta.old_file);
		set_data_from_blob(old_blob, &d.patch.new_data, &d.delta.new_file);
	} else {
		set_data_from_blob(old_blob, &d.patch.old_data, &d.delta.old_file);
		set_data_from_buffer(buf, buflen, &d.patch.new_data, &d.delta.new_file);
	}

	return diff_single_apply(&d);
}
		NULL, diff_patch_hunk_cb, diff_patch_line_cb, NULL);
		ctxt.payload = patch;
			error = ctxt.error;
    size_t content_len,
    void *payload)
	git_buf *output = payload;
	git_diff_data_cb print_cb,
	void *payload)
	pi.payload  = payload;
	error = print_patch_file(patch->delta, 0, &pi);
		error = print_patch_hunk(
			patch->delta, &hunk->range, hunk->header, hunk->header_len, &pi);
				patch->delta, &hunk->range,
				line->origin, line->ptr, line->len, &pi);
	error = git_diff_patch_print(patch, print_to_buffer_cb, &output);

int git_diff__paired_foreach(
	git_diff_list *idx2head,
	git_diff_list *wd2idx,
	int (*cb)(git_diff_delta *i2h, git_diff_delta *w2i, void *payload),
	void *payload)
{
	int cmp;
	git_diff_delta *i2h, *w2i;
	size_t i, j, i_max, j_max;
	bool icase = false;

	i_max = idx2head ? idx2head->deltas.length : 0;
	j_max = wd2idx   ? wd2idx->deltas.length   : 0;

	if (idx2head && wd2idx &&
		(0 != (idx2head->opts.flags & GIT_DIFF_DELTAS_ARE_ICASE) ||
		 0 != (wd2idx->opts.flags & GIT_DIFF_DELTAS_ARE_ICASE)))
	{
		/* Then use the ignore-case sorter... */
		icase = true;

		/* and assert that both are ignore-case sorted. If this function
		 * ever needs to support merge joining result sets that are not sorted
		 * by the same function, then it will need to be extended to do a spool
		 * and sort on one of the results before merge joining */
		assert(0 != (idx2head->opts.flags & GIT_DIFF_DELTAS_ARE_ICASE) &&
			0 != (wd2idx->opts.flags & GIT_DIFF_DELTAS_ARE_ICASE));
	}

	for (i = 0, j = 0; i < i_max || j < j_max; ) {
		i2h = idx2head ? GIT_VECTOR_GET(&idx2head->deltas,i) : NULL;
		w2i = wd2idx   ? GIT_VECTOR_GET(&wd2idx->deltas,j)   : NULL;

		cmp = !w2i ? -1 : !i2h ? 1 :
			STRCMP_CASESELECT(icase, i2h->old_file.path, w2i->old_file.path);

		if (cmp < 0) {
			if (cb(i2h, NULL, payload))
				return GIT_EUSER;
			i++;
		} else if (cmp > 0) {
			if (cb(NULL, w2i, payload))
				return GIT_EUSER;
			j++;
		} else {
			if (cb(i2h, w2i, payload))
				return GIT_EUSER;
			i++; j++;
		}
	}

	return 0;
}
