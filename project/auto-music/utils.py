def list_get(list_, index, default=None):
    if len(list_) > index:
        return list_[index]
    else:
        return default
